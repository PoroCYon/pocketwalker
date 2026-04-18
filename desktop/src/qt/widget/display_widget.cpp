#include "display_widget.h"
#include <cstring>
#include <QFile>
#include "desktop/src/qt/settings/app_settings.h"

#define WIN_W (SCREEN_W * SCALE + MARGIN * 2)
#define WIN_H (SCREEN_H * SCALE + MARGIN * 2)

#define X0 (static_cast<float>(MARGIN) / WIN_W * 2.0f - 1.0f)
#define X1 (X0 + static_cast<float>(SCREEN_W * SCALE) / WIN_W * 2.0f)
#define Y0 (static_cast<float>(MARGIN) / WIN_H * 2.0f - 1.0f)
#define Y1 (Y0 + static_cast<float>(SCREEN_H * SCALE) / WIN_H * 2.0f)

DisplayWidget::DisplayWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    setFixedSize(WIN_W, WIN_H);

    if (QFile splash_file(":/data/splash.bin"); splash_file.open(QIODevice::ReadOnly))
    {
        const QByteArray data = splash_file.readAll();
        memcpy(splash, data.constData(), sizeof(splash));
    }
}

void DisplayWidget::setEmulator(PocketWalker* emu)
{
    emulator = emu;
    was_last_frame_power_save = false;
}

void DisplayWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glGenTextures(1, &fb_texture);
    glBindTexture(GL_TEXTURE_2D, fb_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_W, SCREEN_H, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/framebuffer.vert");
    shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/framebuffer.frag");
    shader.link();

    constexpr float verts[] = {
        X0, Y1, 0.0f, 0.0f,
        X1, Y1, 1.0f, 0.0f,
        X1, Y0, 1.0f, 1.0f,
        X0, Y1, 0.0f, 0.0f,
        X1, Y0, 1.0f, 1.0f,
        X0, Y0, 0.0f, 1.0f,
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void DisplayWidget::drawPixels(const uint8_t* pixels)
{
    glBindTexture(GL_TEXTURE_2D, fb_texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, SCREEN_W, SCREEN_H, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    shader.bind();
    shader.setUniformValue("uTexture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fb_texture);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    shader.release();
}

void DisplayWidget::paintGL()
{
    const auto& palette = AppSettings::instance.emulation.palette;

    glClearColor(palette[0].r / 255.0f, palette[0].g / 255.0f, palette[0].b / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    uint8_t pixels[SCREEN_W * SCREEN_H * 4];

    if (!emulator)
    {
        for (int y = 0; y < SCREEN_H; y++)
        {
            const int page_offset = (y / 8) * 96 * SSD1854_COLUMN_SIZE;
            const int bit = y % 8;
            for (int x = 0; x < SCREEN_W; x++)
            {
                const int base = SSD1854_COLUMN_SIZE * x + page_offset;
                const uint8_t idx = (((splash[base] >> bit) & 1) << 1) |
                    ((splash[base + 1] >> bit) & 1);
                const int i = (y * SCREEN_W + x) * 4;
                pixels[i] = palette[idx].r;
                pixels[i + 1] = palette[idx].g;
                pixels[i + 2] = palette[idx].b;
                pixels[i + 3] = 255;
            }
        }
        drawPixels(pixels);
        return;
    }

    SSD1854DrawInfo* draw_info = emulator->GetDrawInfo();

    if (draw_info->power_save_mode)
    {
        if (!was_last_frame_power_save)
        {
            for (int i = 0; i < SCREEN_W * SCREEN_H; i++)
            {
                pixels[i * 4] = palette[0].r;
                pixels[i * 4 + 1] = palette[0].g;
                pixels[i * 4 + 2] = palette[0].b;
                pixels[i * 4 + 3] = 255;
            }
            drawPixels(pixels);
        }
        was_last_frame_power_save = true;
    }
    else
    {
        for (int y = 0; y < SCREEN_H; y++)
        {
            const int page = y / 8 + draw_info->page_offset;
            const int page_offset = page * SSD1854_TOTAL_COLUMNS * SSD1854_COLUMN_SIZE;
            const int bit_offset = y % 8;
            for (int x = 0; x < SCREEN_W; x++)
            {
                const int base = SSD1854_COLUMN_SIZE * x + page_offset;
                const uint8_t palette_index = (((draw_info->vram.Read8(base) >> bit_offset) & 1) << 1) |
                    ((draw_info->vram.Read8(base + 1) >> bit_offset) & 1);
                const int idx = (y * SCREEN_W + x) * 4;
                pixels[idx] = palette[palette_index].r;
                pixels[idx + 1] = palette[palette_index].g;
                pixels[idx + 2] = palette[palette_index].b;
                pixels[idx + 3] = 255;
            }
        }
        drawPixels(pixels);
        was_last_frame_power_save = false;
    }
}

void DisplayWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}
