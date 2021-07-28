// returns number of characters in string 
int str_len(char *str) {
	int size = 0;

	while (str[size]) {
		size++;
	}

	return size;
}

// compares two strings, returns 1 if same, 0 else
int str_comp(char *str0, char *str1) {
	int out = 1;
	int size0 = str_len(str0);
	int size1 = str_len(str1);

	if (size0 != size1) {
		out = 0;
	} else {
		int idx = 0;

		while (str0[idx]) {
			if (str0[idx] != str1[idx]) {
				out = 0;
				break;
			}

			idx++;
		}
	}

	return out;
}

