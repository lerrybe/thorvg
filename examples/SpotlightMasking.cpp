/*
 * Copyright (c) 2020 - 2025 the ThorVG project. All rights reserved.

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Example.h"

/************************************************************************/
/* ThorVG Drawing Contents                                              */
/************************************************************************/

struct UserExample : tvgexam::Example
{
    // Animation constants
    static constexpr float ANIMATION_DURATION = 3.0f;
    static constexpr float ANIMATION_SPEED = 2.0f * 3.1415f;
    
    // Star shape coordinates
    static constexpr float STAR_CENTER_X = 199.0f;
    static constexpr float STAR_CENTER_Y = 199.0f;
    static constexpr float STAR_POINTS[10][2] = {
        {199, 34},   // top
        {253, 143},  // top-right
        {374, 160},  // right
        {287, 244},  // bottom-right
        {307, 365},  // bottom
        {199, 309},  // bottom-left
        {97, 365},   // left
        {112, 245},  // top-left
        {26, 161},   // far-left
        {146, 143}   // far-top
    };
    
    // Gradient constants
    static constexpr float GRADIENT_START_X = 100.0f;
    static constexpr float GRADIENT_START_Y = 34.0f;
    static constexpr float GRADIENT_END_X = 300.0f;
    static constexpr float GRADIENT_END_Y = 365.0f;
    static constexpr uint8_t COLOR_BASE = 128;
    static constexpr uint8_t COLOR_AMPLITUDE = 127;
    static constexpr float COLOR_PHASE_SHIFT = 1.0f;
    // Mask constants
    static constexpr float MASK_RADIUS = 80.0f;
    static constexpr float MASK_ORBIT_RADIUS = 60.0f;
    static constexpr uint8_t MASK_OPACITY = 255;
    static constexpr uint8_t MASK_COLOR_R = 255;
    static constexpr uint8_t MASK_COLOR_G = 255;
    static constexpr uint8_t MASK_COLOR_B = 255;

    bool content(tvg::Canvas* canvas, uint32_t w, uint32_t h) override
    {
        return update(canvas, 0);
    }

    void compose_star_shape(tvg::Shape* star) 
    {
        star->moveTo(STAR_POINTS[0][0], STAR_POINTS[0][1]);
        star->lineTo(STAR_POINTS[1][0], STAR_POINTS[1][1]);
        star->lineTo(STAR_POINTS[2][0], STAR_POINTS[2][1]);
        star->lineTo(STAR_POINTS[3][0], STAR_POINTS[3][1]);
        star->lineTo(STAR_POINTS[4][0], STAR_POINTS[4][1]);
        star->lineTo(STAR_POINTS[5][0], STAR_POINTS[5][1]);
        star->lineTo(STAR_POINTS[6][0], STAR_POINTS[6][1]);
        star->lineTo(STAR_POINTS[7][0], STAR_POINTS[7][1]);
        star->lineTo(STAR_POINTS[8][0], STAR_POINTS[8][1]);
        star->lineTo(STAR_POINTS[9][0], STAR_POINTS[9][1]);
        star->close();
    }

    void apply_animated_gradient(tvg::Shape* shape, float t, float total_duration)
    {
        auto grad = tvg::LinearGradient::gen();
        grad->linear(GRADIENT_START_X, GRADIENT_START_Y, GRADIENT_END_X, GRADIENT_END_Y);
        
        uint8_t r = (uint8_t)(COLOR_BASE + COLOR_AMPLITUDE * sinf(t * ANIMATION_SPEED / total_duration));
        uint8_t g = (uint8_t)(COLOR_BASE + COLOR_AMPLITUDE * sinf((t + COLOR_PHASE_SHIFT) * ANIMATION_SPEED / total_duration));
        uint8_t b = (uint8_t)(COLOR_BASE + COLOR_AMPLITUDE * sinf((t + COLOR_PHASE_SHIFT * 2.0f) * ANIMATION_SPEED / total_duration));
        
        tvg::Fill::ColorStop stops[3] = {
            {0.0, r, 0, 255, 255},
            {0.5, 0, g, 255, 255},
            {1.0, 0, 0, b, 255}
        };
        grad->colorStops(stops, 3);
        shape->fill(grad);
    }

    void calculate_mask_position(float t, float total_duration, float center_x, float center_y, float& mask_x, float& mask_y)
    {
        mask_x = center_x + MASK_ORBIT_RADIUS * sinf(t * ANIMATION_SPEED / total_duration);
        mask_y = center_y + MASK_ORBIT_RADIUS * cosf(t * ANIMATION_SPEED / total_duration);
    }

    bool update(tvg::Canvas* canvas, uint32_t elapsed) override
    {
        if (!tvgexam::verify(canvas->remove())) return false;

        // 마스킹 위치만 시간에 따라 변경
        float t = fmodf(elapsed / 1000.0f, ANIMATION_DURATION);

        // 별 shape 생성
        auto star = tvg::Shape::gen();
        compose_star_shape(star);

        // 그라디언트 fill (색상 시간에 따라 변화)
        apply_animated_gradient(star, t, ANIMATION_DURATION);

        // 별의 중심 좌표에 맞춰 마스크 위치 계산
        float mask_cx, mask_cy;
        calculate_mask_position(t, ANIMATION_DURATION, STAR_CENTER_X, STAR_CENTER_Y, mask_cx, mask_cy);

        // 마스크 생성 및 적용
        auto mask = tvg::Shape::gen();
        mask->appendCircle(mask_cx, mask_cy, MASK_RADIUS, MASK_RADIUS);
        mask->fill(MASK_COLOR_R, MASK_COLOR_G, MASK_COLOR_B);
        mask->opacity(MASK_OPACITY);
        star->mask(mask, tvg::MaskMethod::Alpha);
        canvas->push(star);

        return true;
    }
};


/************************************************************************/
/* Entry Point                                                          */
/************************************************************************/

int main(int argc, char **argv)
{
    return tvgexam::main(new UserExample, argc, argv);
}
