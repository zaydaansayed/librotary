librotary.a: librotary.o
	ar r $@ $^

librotary.o: librotary.cpp librotary.h
	g++ -std=c++17 -c librotary.cpp -o $@

install: librotary.a
	install -Dm644 $^ /usr/local/lib/$^
	install -Dm644 librotary.h /usr/local/include/librotary.h

uninstall:
	rm -f /usr/local/include/librotary.h
	rm -f /usr/local/lib/librotary.a

clean:
	rm -f librotary.o librotary.a

.PHONY: install uninstall clean
