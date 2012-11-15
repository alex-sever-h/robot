
all: robot

robot: .PHONY
	@echo looking into robot 
	cd robot; $(MAKE) all
	
clean:
	@echo looking into robot 
	cd robot; $(MAKE) clean
	
flash:
	@echo looking into robot 
	cd robot; $(MAKE) flash
	
debug:
	@echo looking into robot 
	cd robot; $(MAKE) debug

.PHONY:
