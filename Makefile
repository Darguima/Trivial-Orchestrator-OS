all: build

build:
	@echo "Building..."
	$(MAKE) build_client
	$(MAKE) build_server
	$(MAKE) -C commands/

build_client:
	@echo "Building client..."
	$(MAKE) -C client/

build_server:
	@echo "Building server..."
	$(MAKE) -C server/

clean:
	@echo "Cleaning..."
	$(MAKE) -C client/ clean
	$(MAKE) -C server/ clean
	$(MAKE) -C commands/ clean