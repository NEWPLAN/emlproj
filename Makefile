PART1= JPEG
PART2= PDF
PART3= GMIME
PART4= REGEX
PART5= SPLITER
PART6=UNZIP

all:$(PART1) $(PART2) $(PART3) $(PART4) $(PART5) $(PART6)
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

test:
	@echo 'test for heres'
	make all;
	@echo '@test for jpeg analyse';
	cd appendix/jpeg; make run ;
	@echo '@test for pdf analyse';
	cd appendix/pdf; make run ;
	@echo '@test for GMIME analyse';
	cd gmime; make run;
	@echo '@test for regex analyse';
	cd regex; make run ;
	@echo '@test for spliter analyse';
	cd spliter ; make run ;
	@echo '@test for zips analyse';
	cd appendix/zips; make run ;
	@echo 'test for demo is OK!';



.PHONY:clean
clean:
	cd appendix/jpeg ; make clean; cd ../.;
	cd appendix/pdf ; make clean; cd ../..;
	cd gmime ; make clean; cd ../..;
	cd regex ; make clean; cd ../..;
	cd spliter ; make clean; cd ..;
	cd appendix/zips ; make clean; cd ..;
	clear ;
	@echo "clean done"

