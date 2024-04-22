#include "pch.h"
#include "CppUnitTest.h"
#include "..\ResizingCircle\ResizingCircle.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(Tests)
	{
	public:
		
		TEST_METHOD(InitCircleTest)
		{
			UINT radius = 50;
			InitCircle();
			Assert::AreEqual(radius, Circle.radius);
			Assert::AreEqual((UCHAR)3, Circle.thickness);
			Assert::AreEqual(Circle.visible, (BOOL)true);

		}

		TEST_METHOD(ResizeCircleUpTest)
		{
			Resizing.vector = Increase;
			Circle.radius = 40;
			Resizing.speed = 5;
			ResizeCircle();
			Assert::AreEqual(Circle.radius,(UINT) 45);

		}

		TEST_METHOD(ResizeCircleDownTest)
		{
			Resizing.vector = Decrease;
			Circle.radius = 40;
			Resizing.speed = 5;
			ResizeCircle();
			Assert::AreEqual(Circle.radius, (UINT)35);

		}


	};
}
