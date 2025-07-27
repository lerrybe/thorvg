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
    bool content(tvg::Canvas* canvas, uint32_t w, uint32_t h) override
    {
        return update(canvas, 0);
    }

    void composeStarShape(tvg::Shape* star) 
    {
        star->moveTo(199, 34);
        star->lineTo(253, 143);
        star->lineTo(374, 160);
        star->lineTo(287, 244);
        star->lineTo(307, 365);
        star->lineTo(199, 309);
        star->lineTo(97, 365);
        star->lineTo(112, 245);
        star->lineTo(26, 161);
        star->lineTo(146, 143);
        star->close();
    }

    void applyAnimatedGradient(tvg::Shape* shape, float t, float totalDuration)
    {
        auto grad = tvg::LinearGradient::gen();
        grad->linear(100, 34, 300, 365);
        uint8_t r = (uint8_t)(128 + 127 * sinf(t * 2 * 3.1415f / totalDuration));
        uint8_t g = (uint8_t)(128 + 127 * sinf((t + 1.0f) * 2 * 3.1415f / totalDuration));
        uint8_t b = (uint8_t)(128 + 127 * sinf((t + 2.0f) * 2 * 3.1415f / totalDuration));
        tvg::Fill::ColorStop stops[3] = {
            {0.0, r, 0, 255, 255},
            {0.5, 0, g, 255, 255},
            {1.0, 0, 0, b, 255}
        };
        grad->colorStops(stops, 3);
        shape->fill(grad);
    }

    bool update(tvg::Canvas* canvas, uint32_t elapsed) override
    {
        if (!tvgexam::verify(canvas->remove())) return false;

        // 마스킹 위치만 시간에 따라 변경
        float totalDuration = 3.0f;
        float t = fmodf(elapsed / 1000.0f, totalDuration);

        // 별 shape 생성
        auto star = tvg::Shape::gen();
        composeStarShape(star);

        // 그라디언트 fill (색상 시간에 따라 변화)
        applyAnimatedGradient(star, t, totalDuration);

        // 별의 중심 좌표에 맞춰 마스크 위치 계산
        float star_cx = 199.0f;
        float star_cy = 199.0f;
        float mask_radius = 80.0f;
        float mask_cx = star_cx + 60.0f * sinf(t * 2 * 3.1415f / totalDuration);
        float mask_cy = star_cy + 60.0f * cosf(t * 2 * 3.1415f / totalDuration);

        auto mask = tvg::Shape::gen();
        mask->appendCircle(mask_cx, mask_cy, mask_radius, mask_radius);
        mask->fill(255, 255, 255);
        mask->opacity(255); // 완전 불투명

        // 마스킹 방법은 Alpha로 고정
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
