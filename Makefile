all: build

build:
	@echo "Building..."
	$(MAKE) build_client
	$(MAKE) build_orchestrator

client: build_client
orchestrator: build_orchestrator

build_client:
	@echo "Building client..."
	$(MAKE) -C client/

build_orchestrator:
	@echo "Building orchestrator..."
	$(MAKE) -C server/

clean:
	@echo "Cleaning..."
	$(MAKE) -C client/ clean
	$(MAKE) -C server/ clean