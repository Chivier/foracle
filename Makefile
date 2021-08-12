gen:
	g++ -O2 test-generate.cpp -o generator

plan1:
	g++ -O2 intrinsic.cpp -o intrinsic

plan2:
	g++ -O2 greed.cpp -o greed

plan3:
	g++ -O2 dp.cpp -o dp

clean:
	rm -rf generator intrinsic greed dp