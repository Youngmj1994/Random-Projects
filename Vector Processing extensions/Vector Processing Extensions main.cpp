/*
when changing from debug to release mode, the compiler changes how it does things. 
in debug, it behaves exactly like you think it should.
in release mode though, things change. take
for(int i=0;i<200;i++)
	z[i] = x[i]+y[i];
it will seperate the additions into multiple load, add, store functions, it can change it to 8 additions per loop so instead of 200 loops its only 25 loops.

vector processing - sse/avx

the ALU can take inputs and an operation (+,-,*,/) to produce a result.
your processor has additional processing sets, which can also take inputs and put out results. but it can't take a unique operation, it has to be the same operation as the ALU has

if you go to properties and go to c++ and go to code generation and go to enable enhanced instruction set, you can change the default to avx or avx2

if you run the code with a breakpoint then go to debug->windows->disassembly you can look at the assembly code.
if you look at the assembly with the avx2 on, you can see an operation called vmovss and vaddss, which are the vector extension mov and add functions

however the change wont happen unless you chnage to release mode instead of debug mode

as a refresh. the cpu looks for memory in its registers, then to the cache, then if its not there look at ram and if its not even there its in the HDD.

now the issue with the compiler is that it cant look at the big picture. it can only look at simple stuff. it runs simpleInstructions just fine but not complexInstructions
it will only use 32 bits, 1 float number, at a time on the complex instructions. its not even using the optimizatons, because its not smart enough to, so it instead does what it knows will work
but its much slower.

so when it comes to coding in vector extensions yourself, you have to break the problem down to its very bare components. all additions, all subtractions, etc.


*/
#include <stdlib.h>

//this is what you need to include to be able to mess aroudn with registers and intrinsics
#include <immintrin.h>

void simpleInstructions()
{
	float* bigArray1 = new float[16384];
	float* bigArray2 = new float[16384];
	float* bigArray3 = new float[16384];

	for (int i = 0; i < 16384; i++)
	{
		bigArray1[i] = 20.0f;
		bigArray2[i] = 50.0f;
		bigArray3[i] = 0.0f;

	}

	for (int i = 0; i < 16384; i++)
	{
		bigArray3[i] = bigArray1[i] + bigArray2[i];
	}

	delete[] bigArray1;
	delete[] bigArray2;
	delete[] bigArray3;
}

void complexInstructions()
{
	float* bigArray1 = new float[16384];
	float* bigArray2 = new float[16384];
	float* bigArray3 = new float[16384];

	for (int i = 0; i < 16384; i++)
	{
		bigArray1[i] = float(rand() % 100);
		bigArray2[i] = 50.0f;
		bigArray3[i] = 0.0f;

	}

	for (int i = 0; i < 16384; i++)
	{
		if (bigArray1[i] == 23.0f)
			bigArray3[i] = bigArray1[i];
		else
			bigArray3[i] = bigArray1[i] + bigArray2[i];
	}

	delete[] bigArray1;
	delete[] bigArray2;
	delete[] bigArray3;
}

void CreateFractalIntrinsics(const olc::vi2d& pix_tl, const olc::vi2d& pix_br, const olc::vd2d& frac_tl, const olc::vd2d& frac_br, const int iterations)
{
	double x_scale = (frac_br.x - frac_tl.x) / (double(pix_br.x) - double(pix_tl.x));
	double y_scale = (frac_br.y - frac_tl.y) / (double(pix_br.y) - double(pix_tl.y));

	double y_pos = frac_tl.y;

	int y_offset = 0;
	int row_size = ScreenWidth();

	int x, y;

	//this is tellig the computer that these are specifically 64-bit "double" registers
	__m256d _a, _b, _two, _four, _mask1;
	__m256d _zr, _zi, _zr2, _zi2, _cr, _ci;
	__m256d _x_pos_offsets, _x_pos, _x_scale, _x_jump; 
	//the _zr2 and _zi2 are zr*zr and zi*zi

	//64-bit "integer" registers
	__m256i _n, _iterations, _mask2, _c, _one; //the masks have to be of the same type as what you are comparing

	// Expand constants into vectors of constants
	// one = |(int)1|(int)1|(int)1|(int)1|		
	_one = _mm256_set1_epi64x(1);

	// two = |2.0|2.0|2.0|2.0|
	_two = _mm256_set1_pd(2.0);

	// four = |4.0|4.0|4.0|4.0|
	_four = _mm256_set1_pd(4.0);

	// iterations = |iterations|iterations|iterations|iterations|
	_iterations = _mm256_set1_epi64x(iterations);

	_x_scale = _mm256_set1_pd(x_scale);
	_x_jump = _mm256_set1_pd(x_scale * 4);
	_x_pos_offsets = _mm256_set_pd(0, 1, 2, 3);
	_x_pos_offsets = _mm256_mul_pd(_x_pos_offsets, _x_scale);

	

	_iterations = _mm256_set1_epi64x(iterations); //set iteration number to a 64 bit int in each _iterations
	_one = _mm256_set1_epi64x(1);


	for (y = pix_tl.y; y < pix_br.y; y++)
	{
		// Reset x_position
		_a = _mm256_set1_pd(frac_tl.x);
		_x_pos = _mm256_add_pd(_a, _x_pos_offsets);

		_ci = _mm256_set1_pd(y_pos);

		for (x = pix_tl.x; x < pix_br.x; x += 4)
		{
			_cr = _x_pos;

			// Zreal = 0
			_zr = _mm256_setzero_pd();

			// Zimag = 0
			_zi = _mm256_setzero_pd();

			// nIterations = 0
			_n = _mm256_setzero_si256();

			repeat:
			_zr2 = _mm256_mul_pd(_zr, _zr); //zr * zr. the _mm256 part says that its using the 256 bit register, the _mul_ part is telling it to multiply, the p part is telling it to do it parallel
			// and the d part is saying that its to use doubles
			_zi2 = _mm256_mul_pd(_zi, _zi); //zi * zi
			_a = _mm256_sub_pd(_zr2, _zi2); //a = ar^2 - zi^2
			_a = _mm256_add_pd(_a, _cr); //a = a + cr
			_b = _mm256_mul_pd(_zr, _zi); // b = zr * zi
			_b = _mm256_fmadd_pd(_b, _two, _ci); //b = b * 2.0 + ci
			//the _fmadd stands for floating point multiply and add
			_zr = _a;
			_zi = _b;

			_a = _mm256_add_pd(_zr2, _zi2); // a = (zr * zr) + (zi * zi)
			//now we have to do a conditional. this requires soemthing called a mask, which is an intrinsic way to hold boolean values
			//m1  = if(a<four)
			//m1 = |if(a[3] < |4.0|)|if(a[2] < |4.0|)|if(a[1] < |4.0|)|if(a[0] < |4.0|)|
			//m1 = |111...111|000...000|111...111|000...000| this will set the mask to true or false depending on the answer and set all 64 bits to either 1 or zero in each register
			_mask1 = _mm256_cmp_pd(_a, _four, _CMP_LT_OQ); //a < 4.0. _cmp_ is compare. _cmp_lt_oq is compare less than, ordered nonsignaling

			//m2 = if(iterations > n). there is no integer less than in this particular library
			// m2 = |00...00|11...11|11...11|00...00|
			_mask2 = _mm256_cmpgt_epi64(_iterations, _n); //greater than

			//m2 = m2 AND m1 = if(a < 4.0 && iterations > n)
			//m2 = |00...|11...|000...|111...|
			_mask2 = _mm256_and_si256(_mask2, _mm256_castpd_si256(_mask1)); //comparing 2 256-bit registers, in this case _mask2 and mask1. (mask1 has to be cast to integer to be compared)
			//the cast doesnt actually cost anything, its just turning the mask into a single integer of size 256
			//now mask2 has only the elements that satisfy both the conditions, so we can increment n, only for those specific pixels that are all 1's 

			//c = |(int)1|(int)1|(int)1|(int)1| AND m2
			//
			// c =		|00...01|00...01|00...01|00...01|
			// m2 = AND |00...00|00...00|11...11|00...00|
			// c =		|00...00|00...00|00...01|00...00|
			//
			// c = |(int)0|(int)0|(int)1|(int)0|
			//this is to make c have the list of elements that passed the two conditions and to set its least significant bit to either one or zero, like a bool

			_c = _mm256_and_si256(_one, _mask2);//this is anding 1 and mask2 to get c the least significant bit
			//this will increment n in the saces where it needs to. so it can look like->
			//n =  |00..24|00..13|00..08|00..21|
			//c = +|00..00|00..00|00..01|00..00|
			//n =  |00..24|00..13|00..09|00..21| it only increments the right elements
			_n = _mm256_add_epi64(_n, _c);

			//if ((zr * zr + zi * zi) < 4.0 && n < iterations) goto repeat
			//i.e. if our mask has any elements that are 1
			//|00...00|00...00|11...11|00...00|
			//|   0   |   0   |   1   |   0   | = 0b0010 = 2
			//so... if(2>0) goto repeat
			if (_mm256_movemask_pd(_mm256_castsi256_pd(_mask2)) > 0) //the movemask takes the most significant bit of the mask and extracts that into 4 bits so 0010 in this case
				goto repeat;
			//the castsi256_pd converts 4 64 bit doubles into one 256 bit int.

			// Tight loop has finished, all 4 pixels have been evaluated. Increment
			// fractal space x positions for next 4 pixels
			// x_pos = x_pos + x_jump
			_x_pos = _mm256_add_pd(_x_pos, _x_jump);

			// Unpack our 4x64-bit Integer Vector into normal 32-bit Integers
			// and write into memory at correct location. Note, depending on
			// how you structure the memory, and the types you use, this step
			// may not be required. If I was working with 64-bit integers I
			// could choose to just write the vector entirely, saving this
			// truncation at the expense of 2x the memory required

			pFractal[y_offset + x + 0] = int(_n.m256i_i64[3]);
			pFractal[y_offset + x + 1] = int(_n.m256i_i64[2]);
			pFractal[y_offset + x + 2] = int(_n.m256i_i64[1]);
			pFractal[y_offset + x + 3] = int(_n.m256i_i64[0]);
		} 
		y_pos += y_scale;
		y_offset += row_size;
	}
	/*
	int n = 0;
	while(abs(z) < 2.0 && n < iterations)

	or 

	while((zr * zr + zi * zi) < 4.0 && n < iterations)
	*/
	


}

int main()
{
	simpleInstructions();
	complexInstructions();
	return 0;
}