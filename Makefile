all: build

build:
	@echo "Building..."
	$(MAKE) build_client
	$(MAKE) build_server
	$(MAKE) compile_commands

build_client:
	@echo "Building client..."
	$(MAKE) -C client/ -Idatapipe

build_server:
	@echo "Building server..."
	$(MAKE) -C server/

compile_commands:
	@echo "Building command binaries..."
	$(MAKE) -C bin/

clean:
	@echo "Cleaning..."
	$(MAKE) -C client/ clean
	$(MAKE) -C server/ clean
	$(MAKE) -C bin/ clean