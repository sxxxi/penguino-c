#include"../headers/action_registry.h"
#include<stdio.h>

void setup();

int ex(int a) {
	printf("Hello %d\n", a);
}

int main() {
	setup();

	char buffer[32];

	for (int i = 0; i < 200; i++) {
		snprintf(buffer, sizeof buffer, "FOO%d", i);
		registry_entry* e = new_registry_entry(buffer, ex);
		add_entry(e);	
	}

	snprintf(buffer, sizeof buffer, "FOO%d", 150);
	printf("%sHi\n", buffer);
	action_lambda l = get(buffer);
	l(5);

    return 0;
}

void setup() {
	init_registry();	
}
