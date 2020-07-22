//1280 x 720 screen size
//0.0,0.0 -> 2.0,4.0 world space
//work out the screen space point by taking the size of the world space say 0.5/720 pixels
//this gives the prescise world space point, which tells us what the fractal looks like at that coordinate. 
//the problem is that numbers will get really small, like .00000000000000000001, etc, so its best to use doubles for its 64 bit precision;
//pseudo code from wikipedia for the escape time algorithm for plotting a fractal

/*
for each pixel(Px, Py) on the screen do
    x0 = scaled x coordinate of pixel (scaled to lie in Mandelbrot X scale (-2.5,1))
    y0 = scaled y coordinate of pixel (scaled to lie in Mandelbrot Y scale (-1,1))
    x:= 0.0
    y:= 0.0
    max_iteration := 1000
    while(x*x + y*y <= 2*2 && iteration < max_iteration) do
        xtemp := x*x - y*y + x0
        y:= 2*x*y + y0
        x:= xtemp
        iteration++
    color := palette[iteration]
    plot(Px,Py, color)


frequency is 1/Time which can give you frames per second, which is accurate but also not intuitive on how to fix

AVX-256 is a processor extension, which allows parralelleism.
the AVX is a 256 bit register. and we are using doubles for 64bit precision which allows us to fit 4 doubles into one register. 32bit float is 8, etc.
the vector processor follows SIMD Single Instruction Multiple Data. which means you can have multiple bits of data(4 doubles) but they all have to have single instructions put onto them (+,-,*,/,etc)

so what you can do is use this optimization to perform multiple simple instructions on multiple bits of data, and you can thread it, using more cores. so you can get 4 doubles and then
put that into 4 threads so you can do 16 doubles at the exact same time, increasign the speed by a large magnitude compared to the naive implementation.

*/
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <complex>
#include <atomic>
#include <cstdlib>
#include <immintrin.h>
#include <condition_variable>

constexpr int nMaxThreads = 32;

class Mandelbrot : public olc::PixelGameEngine
{
public:
    Mandelbrot()
    {
        sAppName = "Mandelbrot stuff";
    }
    int* pFractal = nullptr;
    int nMode = 4;
    int nIterations = 128;

public:
    bool OnUserCreate() override
    {
        //pFractal = new int[ScreenWidth() * ScreenHeight()]{0};
        //points the pointer to a contiguous amount of memeory the size of the screen height*width*the size of int, on alignment 64.
        pFractal = (int*)_aligned_malloc(size_t(ScreenWidth()) * size_t(ScreenHeight()) * sizeof(int), 64);

        //InitialiseThreadPool();
        return true;
    }

    /*
    bool OnUserDestroy() override
    {
        //stop worker threads
        for (int i = 0; i < nMaxThreads; i++)
        {
            workers[i].alive = false;
            workers[i]cvStart.notify_one();
        }

        //clean up worker threads
        for (int i = 0; i < nMaxThreads; i++)
        {
            workers[i].thread.join();
        }

        //free all the memory that pfractal is pointing to
        _aligned_free(pFractal);
        return true;
    }
    */

    //first method, unoptimized
    void CreateFractalBasic(const olc::vi2d& pix_tl, const olc::vi2d& pix_br, const olc::vd2d& frac_tl, const olc::vd2d& frac_br, const int iterations)
    {
        //scaling x and y through the convience of xy position of screen space / xy position of world space = a workable scale
        double x_scale = (frac_br.x - frac_tl.x) / (double(pix_br.x) - double(pix_tl.x));
        double y_scale = (frac_br.y - frac_tl.y) / (double(pix_br.y) - double(pix_tl.y));

        for (int y = pix_tl.y; y < pix_br.y; y++)
        {
            for (int x = pix_tl.x; x < pix_br.x; x++)
            {
                //complex is for precision safeguarding, can't go to anything other than a lower precision
                std::complex<double> c(x * x_scale + frac_tl.x, y * y_scale + frac_tl.y);
                std::complex<double> z(0, 0);

                int n = 0;
                while (abs(z) < 2.0 && n < iterations)
                {
                    //the value of z is the distance from z to 0,0 in the complex plane C
                    z = (z * z) + c;
                    n++;
                }
                pFractal[y * ScreenWidth() + x] = n;
            }
        }
    }

    //second method, pre-calculate as much as you can,to avoid doing the same work over again
    //no real optimization, the compiler is basically doing this already. maybe a small amount of optimization
    void CreateFractalPreCalculate(const olc::vi2d& pix_tl, const olc::vi2d& pix_br, const olc::vd2d& frac_tl, const olc::vd2d& frac_br, const int iterations)
    {
        double x_scale = (frac_br.x - frac_tl.x) / (double(pix_br.x) - double(pix_tl.x));
        double y_scale = (frac_br.y - frac_tl.y) / (double(pix_br.y) - double(pix_tl.y));

        double x_pos = frac_tl.x;
        double y_pos = frac_tl.y;

        int y_offset = 0;
        int row_size = pix_br.x - pix_tl.x;

        int x, y, n;
        std::complex<double> c, z;

        for (y = pix_tl.y; y < pix_br.y; y++)
        {
            x_pos = frac_tl.x;
            for (x = pix_tl.x; x < pix_br.x; x++)
            {
                c = { x_pos,y_pos };
                z = { 0,0 };

                n = 0;
                while (abs(z) < 2.0 && n < iterations)
                {
                    z = (z * z) + c;
                    n++;
                }

                pFractal[y_offset + x] = n;
                x_pos += x_scale;
            }
            y_pos += y_scale;
            y_offset += row_size;
        }
    }

    //method 3 getting rid of complex and just hard coding the math
    void CreateFractalNoComplex(const olc::vi2d& pix_tl, const olc::vi2d& pix_br, const olc::vd2d& frac_tl, const olc::vd2d& frac_br, const int iterations)
    {
        double x_scale = (frac_br.x - frac_tl.x) / (double(pix_br.x) - double(pix_tl.x));
        double y_scale = (frac_br.y - frac_tl.y) / (double(pix_br.y) - double(pix_tl.y));

        double x_pos = frac_tl.x;
        double y_pos = frac_tl.y;

        int y_offset = 0;
        int row_size = ScreenWidth();

        int x, y, n;

        //C plane real, c plane imaginary
        double cr = 0;
        double ci = 0;
        double zr = 0;
        double zi = 0;
        double re = 0;
        double im = 0;

        for (y = pix_tl.y; y < pix_br.y; y++)
        {
            x_pos = frac_tl.x;
            ci = y_pos;
            for (x = pix_tl.x; x < pix_br.x; x++)
            {
                cr = x_pos;
                zr = 0;
                zi = 0;

                n = 0;
                //Zreal^2 + Zimaginary^2 < 4.0. this is a distance formula, finding the distance from 0 to that numvber on the complex plane
                //the only real optimization here is replacing abs(z) with the multiplicationa nd addition. but you lose out on generality by doing this.
                while ((zr * zr + zi * zi) < 4.0 && n < iterations)
                {
                    re = zr * zr - zi * zi + cr;
                    im = zr * zi * 2.0 + ci;
                    zr = re;
                    zi = im;
                    n++;
                }
                pFractal[y_offset + x] = n;
                x_pos += x_scale;
            }
            y_pos += y_scale;
            y_offset += row_size;
        }
    }


    //method 4 using AVX2 Vector co-processor to handle 4 fractal locations at once
    void CreateFractalIntrinsics(const olc::vi2d& pix_tl, const olc::vi2d& pix_br, const olc::vd2d& frac_tl, const olc::vd2d& frac_br, const int iterations)
    {
        double x_scale = (frac_br.x - frac_tl.x) / (double(pix_br.x) - double(pix_tl.x));
        double y_scale = (frac_br.y - frac_tl.y) / (double(pix_br.y) - double(pix_tl.y));

        double y_pos = frac_tl.y;

        int y_offset = 0;
        int row_size = ScreenWidth();

        int x, y;

        __m256d _a, _b, _two, _four, _mask1, _zr, _zi, _zr2, _zi2, _cr, _ci, _x_pos_offsets, _x_pos, _x_scale, _x_jump;
        __m256i _one, _c, _n, _iterations, _mask2;

        _one = _mm256_set1_epi64x(1);
        _two = _mm256_set1_pd(2.0);
        _four = _mm256_set1_pd(4.0);
        _iterations = _mm256_set1_epi64x(iterations);

        _x_scale = _mm256_set1_pd(x_scale);
        _x_jump = _mm256_set1_pd(x_scale * 4);
        _x_pos_offsets = _mm256_set_pd(0, 1, 2, 3);
        _x_pos_offsets = _mm256_mul_pd(_x_pos_offsets, _x_scale);

        for (y = pix_tl.y; y < pix_br.y; y++)
        {
            //reset x position
            _a = _mm256_set1_pd(frac_tl.x);
            _x_pos = _mm256_add_pd(_a, _x_pos_offsets);
            _ci = _mm256_set1_pd(y_pos);

            for (x = pix_tl.x; x < pix_br.x; x += 4)
            {
                //this is basically the same as the implementations above but each instruction can only be done one at a time

                _cr = _x_pos;
                _zr = _mm256_setzero_pd();
                _zi = _mm256_setzero_pd();
                _n = _mm256_setzero_si256();

                //you have to do this because of the binary weirdness you ahve to do to escape a loop
            repeat:
                _zr2 = _mm256_mul_pd(_zr, _zr);
                _zi2 = _mm256_mul_pd(_zi, _zi);
                _a = _mm256_sub_pd(_zr2, _zi2);
                _a = _mm256_add_pd(_a, _cr);
                _b = _mm256_mul_pd(_zr, _zi);
                _b = _mm256_fmadd_pd(_b, _two, _ci);
                _zr = _a;
                _zi = _b;
                _a = _mm256_add_pd(_zr2, _zi2);
                _mask1 = _mm256_cmp_pd(_a, _four, _CMP_LT_OQ);
                _mask2 = _mm256_cmpgt_epi64(_iterations, _n);
                _mask2 = _mm256_and_si256(_mask2, _mm256_castpd_si256(_mask1));
                _c = _mm256_and_si256(_one, _mask2); // Zero out ones where n < iterations													
                _n = _mm256_add_epi64(_n, _c); // n++ Increase all n
                //binary weirdness
                if (_mm256_movemask_pd(_mm256_castsi256_pd(_mask2)) > 0)
                    goto repeat;

                //upack all the arrays of pixels into the pfractl
                pFractal[y_offset + x + 0] = int(_n.m256i_i64[3]);
                pFractal[y_offset + x + 1] = int(_n.m256i_i64[2]);
                pFractal[y_offset + x + 2] = int(_n.m256i_i64[1]);
                pFractal[y_offset + x + 3] = int(_n.m256i_i64[0]);
                _x_pos = _mm256_add_pd(_x_pos, _x_jump);
            }
            y_pos += y_scale;
            y_offset += row_size;
        }
    }

    //method 5 time to do threading. use multiple register vectors with multiple threads to boost this even higher at the cost of cpu power
    //take the entire screen and partition it into the number of threads
    void CreateFractalThreads(const olc::vi2d& pix_tl, const olc::vi2d& pix_br, const olc::vd2d& frac_tl, const olc::vd2d& frac_br, const int iterations)
    {
        int nSectionWidth = (pix_br.x - pix_tl.x) / nMaxThreads;
        double dFractalWidth = (frac_br.x - frac_tl.x) / double(nMaxThreads);

        std::thread t[nMaxThreads];

        for (size_t i = 0; i < nMaxThreads; i++)
        {
            t[i] = std::thread(&Mandelbrot::CreateFractalIntrinsics, this,
                olc::vi2d(pix_tl.x + nSectionWidth * (i), pix_tl.y),
                olc::vi2d(pix_tl.x + nSectionWidth * (i + 1), pix_br.y),
                olc::vd2d(frac_tl.x + dFractalWidth * double(i), frac_tl.y),
                olc::vd2d(frac_tl.x + dFractalWidth * double(i + 1), frac_br.y),
                iterations);
        }
        for (size_t x = 0; x < nMaxThreads; x++)
            t[x].join();
        //join, for refrence basically waits for all threads to be done before executing its operation
        //"This synchronizes the moment this function returns with the completion of all the operations in the thread"
        //the number of threads is normally 32, we can do this because not all threads will complete at the same time
    }

    /*
    method 6 would be to make a threadpool, to avoid the overhead of constantly creating and exiting threads.
    the threadpool always has these threads in existance, ready to go. do it once at the start of the thread and then have them wait

    then method 7 would be to optimize the actual mandelbrot calculatiosn itself, scale the values of the floating points based on the zoom so that the issue of precision is not a limitation.
    This would allow you to zoom indefinitely. and there is no need for the computation to go to the max iteration count for pixels which are clearly in the middle region and will go
    forever if possible.

    method 8 would be to do all this with a gpu, which is specifically designed to do this kind of thing


    */
    //this convert coordinates from screen space to world space
    void ScreenToWorld(const olc::vi2d& n, olc::vd2d& v)
    {
        v.x = (double)(n.x) / vScale.x + vOffset.x;
        v.y = (double)(n.y) / vScale.y + vOffset.y;
    }



    bool OnUserUpdate(float fElapsedTime) override
    {

        // Get mouse location this frame
        olc::vd2d vMouse = { (double)GetMouseX(), (double)GetMouseY() };

        // Handle Pan & Zoom
        if (GetMouse(2).bPressed)
        {
            vStartPan = vMouse;
        }

        if (GetMouse(2).bHeld)
        {
            vOffset -= (vMouse - vStartPan) / vScale;
            vStartPan = vMouse;
        }

        olc::vd2d vMouseBeforeZoom;
        ScreenToWorld(vMouse, vMouseBeforeZoom);

        if (GetKey(olc::Key::Q).bHeld || GetMouseWheel() > 0) vScale *= 1.1;
        if (GetKey(olc::Key::A).bHeld || GetMouseWheel() < 0) vScale *= 0.9;

        olc::vd2d vMouseAfterZoom;
        ScreenToWorld(vMouse, vMouseAfterZoom);
        vOffset += (vMouseBeforeZoom - vMouseAfterZoom);

        olc::vi2d pix_tl = { 0,0 };
        olc::vi2d pix_br = { ScreenWidth(), ScreenHeight() };
        olc::vd2d frac_tl = { -2.0, -1.0 };
        olc::vd2d frac_br = { 1.0, 1.0 };

        ScreenToWorld(pix_tl, frac_tl);
        ScreenToWorld(pix_br, frac_br);

        // Handle User Input
        if (GetKey(olc::K1).bPressed) nMode = 0;
        if (GetKey(olc::K2).bPressed) nMode = 1;
        if (GetKey(olc::K3).bPressed) nMode = 2;
        if (GetKey(olc::K4).bPressed) nMode = 3;
        if (GetKey(olc::K5).bPressed) nMode = 4;
        if (GetKey(olc::K6).bPressed) nMode = 5;
        if (GetKey(olc::UP).bPressed) nIterations += 64;
        if (GetKey(olc::DOWN).bPressed) nIterations -= 64;
        if (nIterations < 64) nIterations = 64;


        // START TIMING
        auto tp1 = std::chrono::high_resolution_clock::now();

        // Do the computation
        switch (nMode)
        {
        case 0: CreateFractalBasic(pix_tl, pix_br, frac_tl, frac_br, nIterations); break;
        case 1: CreateFractalPreCalculate(pix_tl, pix_br, frac_tl, frac_br, nIterations); break;
        case 2: CreateFractalNoComplex(pix_tl, pix_br, frac_tl, frac_br, nIterations); break;
        case 3: CreateFractalIntrinsics(pix_tl, pix_br, frac_tl, frac_br, nIterations); break;
        case 4: CreateFractalThreads(pix_tl, pix_br, frac_tl, frac_br, nIterations); break;
            //case 5: CreateFractalThreadPool(pix_tl, pix_br, frac_tl, frac_br, nIterations); break;
        }

        // STOP TIMING
        auto tp2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsedTime = tp2 - tp1;


        // Render result to screen
        for (int y = 0; y < ScreenHeight(); y++)
        {
            for (int x = 0; x < ScreenWidth(); x++)
            {
                int i = pFractal[y * ScreenWidth() + x];
                float n = (float)i;
                float a = 0.1f;
                // Thank you @Eriksonn - Wonderful Magic Fractal Oddball Man
                Draw(x, y, olc::PixelF(0.5f * sin(a * n) + 0.5f, 0.5f * sin(a * n + 2.094f) + 0.5f, 0.5f * sin(a * n + 4.188f) + 0.5f));
            }
        }

        // Render UI
        switch (nMode)
        {
        case 0: DrawString(0, 0, "1) Naive Method", olc::WHITE, 3); break;
        case 1: DrawString(0, 0, "2) Precalculate Method", olc::WHITE, 3); break;
        case 2: DrawString(0, 0, "3) Hand-code Maths Method", olc::WHITE, 3); break;
        case 3: DrawString(0, 0, "4) Vector Extensions (AVX2) Method", olc::WHITE, 3); break;
        case 4: DrawString(0, 0, "5) Threads Method", olc::WHITE, 3); break;
        case 5: DrawString(0, 0, "6) ThreadPool Method", olc::WHITE, 3); break;
        }

        DrawString(0, 30, "Time Taken: " + std::to_string(elapsedTime.count()) + "s", olc::WHITE, 3);
        DrawString(0, 60, "Iterations: " + std::to_string(nIterations), olc::WHITE, 3);
        return !(GetKey(olc::Key::ESCAPE).bPressed);
    }

    olc::vd2d vOffset = { 0.0,0.0 };
    olc::vd2d vStartPan = { 0.0,0.0 };
    olc::vd2d vScale = { 1280.0 / 2.0,720.0 };
};


int main()
{
    Mandelbrot demo;
    if (demo.Construct(1280, 720, 1, 1, false, false))
        demo.Start();
    return 0;
}