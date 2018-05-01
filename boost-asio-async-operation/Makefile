CXXFLAGS += -std=c++14 -DBOOST_COROUTINE_NO_DEPRECATION_WARNING -DBOOST_COROUTINES_NO_DEPRECATION_WARNING
LDLIBS += -lboost_coroutine -lboost_context -lboost_system -pthread

.PHONY: boost-asio-async-operation

boost-asio-async-operation: main.o
	$(LINK.cc) $^ $(LDLIBS) -o $@
