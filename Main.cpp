#include "Operation.h"

int main(int argc, const char *argv[]) {
	VideoCapture capture(argv[1]);

	if(!capture.isOpened()) {
		cout << format("\n %s could not be opened!", argv[1]);
		return EXIT_FAILURE;
	}

	cout << format("\n Capture was initiated in %s", argv[1]);
	Operation operation(capture);
	return EXIT_SUCCESS;
}
