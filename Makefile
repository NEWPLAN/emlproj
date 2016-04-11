PART1= JPEG
PART2= PDF
PART3= GMIME
PART4= REGEX
PART5= SPLITER
PART6=UNZIP


all:
	cd main/; make;
	@echo 'make done!'
$(PART1):
	cd appendix/jpeg; make

$(PART2):
	cd appendix/pdf; make

$(PART3):
	cd gmime; make

$(PART4):
	cd regex; make

$(PART5):
	cd spliter; make

$(PART6):
	cd appendix/zips;make
run:
	@echo 'test for heres'
	cd main/; make test;



.PHONY:clean
clean:
	cd main/; make clean;
	clear ;
	@echo "clean done"

