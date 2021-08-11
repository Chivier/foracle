gen:
	g++ -O2 test-generate.cpp -o generator

plan1:
	g++ -O2 intrinsic.cpp -o intrinsic

plan2:
	g++ -O2 greed.cpp -o greed

clean:
	rm -rf generator intrinsic greed