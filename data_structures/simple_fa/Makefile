demo:
	python3 ./simple_fa.py --verbose --visualize --input sample1.txt
	python3 ./simple_fa.py --verbose --input sample2.txt
	python3 ./simple_fa.py --verbose --input sample3.txt ||:
	python3 ./simple_fa.py --verbose --input sample4.txt ||:

clean:
	rm -f table.dot.pdf
	rm -f table.dot
