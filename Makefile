gen:
	g++ -O2 test-generate.cpp -o generator

plan1:
	g++ -g intrinsic.cpp -o intrinsic

plan2:
	g++ -g greed.cpp -o greed

plan3:
	g++ -g dp.cpp -o dp

clean:
	rm -rf generator intrinsic greed dp