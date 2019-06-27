#include <stdio.h>
#include <conio.h>
//for malloc
#include <stdlib.h>

#define MAX_INTERVAL 20
#define RND_INTERVAL 10 

struct Strc{
	int value;
	char name_user[100];
};

void SubString(char *tmp, char *line, int start, int end){
	int z = 0;
	for (int i = start; z < end; i++){
		tmp[z] = line[i];
		z++;
	}
	tmp[end] = '\0';
}

void swap(int *a, int *b){
	int x = *a;
	*a = *b;
	*b = x;
}

void swap_char(char *str1, char *str2){
	char *temp = malloc((strlen(str1) + 1) * sizeof(char));
	strcpy(temp, str1);
	strcpy(str1, str2);
	strcpy(str2, temp);
	free(temp);
}

void Sort_Bubble(struct Strc *struct1, size_t n){
	int i, j;
	for (i = 0; i < n; ++i){
		for (j = i + 1; j < n; ++j){
			if (struct1[i].value < struct1[j].value){
				swap(&struct1[i].value, &struct1[j].value);
				swap_char(&struct1[i].name_user, &struct1[j].name_user);
			}
		}
	}
}

void AppendSymbol(char *word, char c){
	size_t len = strlen(word);
	word[len] = c;
	word[len + 1] = '\0';
}

void Check_Memory(char* mem){
	if (mem == NULL) {
		printf("\nMemory allocation failure\n");
		return 1;
	}
}

// Game
void Write_Score(char* name, int score){
	FILE *name_file;
	char file_link[] = "C:/Users/Alex/Desktop/Kodding/C/Hangman/score.txt";
	char t_1[] = "Name";
	char t_2[] = "Score";
	struct Strc *struct1 = malloc(sizeof(struct Strc)); // 8
	if (struct1 == NULL){
		printf("\nMemory allocation failure\n");
		return 1;
	}
	size_t struct_i = 0;
	size_t file_empty = 1;

	fopen_s(&name_file, file_link, "r+");

	fseek(name_file, 0, SEEK_END);
	int pos = ftell(name_file);

	if (pos>0){ // Save data to struct
		rewind(name_file); // to start
		char c;
		int word_ln;
		int word_start = 0;
		size_t skip_line = 0;
		size_t check_start = 0;
		size_t all_word_length = 1;
		size_t struct_size = 1;

		char *line = malloc(sizeof(char));
		Check_Memory(line);
		char *tmp_num = malloc(sizeof(char));
		Check_Memory(tmp_num);

		// Add currently score
		SubString(struct1[struct_i].name_user, name, 0, strlen(name));
		struct1[struct_i].value = score;
		struct_size++;
		struct_i++;

		*line = '\0';
		while ((c = fgetc(name_file)) != EOF){
			if (c == '\n' && skip_line == 0){
				skip_line++;
			}
			else if (skip_line > 0){
				all_word_length++;
				line = realloc(line, all_word_length * sizeof(char));
				Check_Memory(line);
				AppendSymbol(line, c);

				if (c == '\t'){
					// Names add to struct
					word_ln = strlen(line) - word_start - 1; // Finish - start - symbol

					struct1 = realloc(struct1, struct_size * sizeof(struct Strc));
					if (struct1 == NULL){
						printf("\nMemory allocation failure\n");
						return 1;
					}
					SubString(struct1[struct_i].name_user, line, word_start, word_ln);

					check_start--;
					continue;
				}
				else if (c == '\n'){
					// Scores add to struct
					word_ln = strlen(line) - word_start - 1;
					tmp_num = realloc(tmp_num, (word_ln + 1) * sizeof(char));
					Check_Memory(tmp_num);

					SubString(tmp_num, line, word_start, word_ln);
					sscanf_s(tmp_num, "%d", &struct1[struct_i].value);

					check_start--;
					struct_size++;
					struct_i++;
					continue;
				}

				if (check_start == 0){
					word_start = all_word_length - 2;
					check_start++;
				}
			}
		}
		// Sort values
		Sort_Bubble(struct1, struct_i);
		
		free(tmp_num);
		free(line);
	}
	else{
		// File is empty
		fprintf(name_file, "%s\t%s\n", t_1, t_2);
		fprintf(name_file, "%s\t%d\n", name, score);
		file_empty = 0;
	}
	fclose(name_file);

	if (file_empty){
		// ReWrite all results
		FILE *name_file_write;
		fopen_s(&name_file_write, file_link, "w");
		size_t print_i = 0;
		for (int i = 0; i < struct_i; i++){
			if (print_i == 0){
				fprintf(name_file_write, "Name\tScore\n");
				print_i++;
				i--;
			}
			else fprintf(name_file_write, "%s\t%d\n", struct1[i].name_user, struct1[i].value);
		}
		fclose(name_file_write);
	}

	free(struct1);
}

void Finish(){
	printf("\nPlease, press Enter to back to menu\n");
	int symbol = 0;
	switch (symbol = _getch()){
	case 13:
		return;
	}
}

void Continue_Game(int *z){
	printf("\nPlease, press Enter to continue\n");
	printf("Or '1' to exit");
	int symbol = 0;
	switch (symbol = _getch()){
	case 13:
		return;
	case 49:
		*z = 0;
	}
}

void ReDraw(char* correct){
	system("@cls||clear");
	printf("\n\n\t\t\t\t*** Hangman ***\n\n\n");
	for (int i = 0; i < strlen(correct); i++){
		printf("\t\t %c ", correct[i]);
	}
}

void ReadFile(){
	int text;
	int cnt_score = 0;
	size_t size = 5;
	size_t read_size = 0;
	size_t cnt_word_score = 0;

	char *name = malloc(size);
	Check_Memory(name);
	name[0] = '\0';

	while (1){
		system("@cls||clear");
		printf("\n\n\t\t\t\t*** Hangman ***\n\n\n");
		printf("\nType '1' to exit or\n");
		printf("Type your name:\n");
		text = fgetc(stdin);
		while (text != EOF && text != '\n'){
			name[read_size] = text;
			read_size++;
			if (read_size == size){
				size += 5;
				name = realloc(name, size);
				Check_Memory(name);
			}
			text = fgetc(stdin);
		}
		name[read_size] = '\0'; //printf("%s", name);

		if (strcmp(name, "1") == 0){
			free(name);
			return;
		}
		else if (strcmp(name, "") == 0){
			free(name);
			printf("\nCorrectly type you name\n");
			printf("Press Enter...\n");
			_getch();
			return;
		}
		else break;
	}

	int tmp_continue = 1;
	int tmp_finish = 1;

	// Random numbers
	int rand_number = 0;
	int num_array[MAX_INTERVAL], vektor_f[RND_INTERVAL];

	for (int val = 0; val < MAX_INTERVAL; val++){ // Interval of numbers
		num_array[val] = val;
	}
	for (int i = 0; i < RND_INTERVAL; i++){ // Pick up numbers - final
		int j = i + rand() % (RND_INTERVAL - i); // Interval of numbers 
		int temp = num_array[i];
		num_array[i] = num_array[j];
		num_array[j] = temp;

		vektor_f[i] = num_array[i];
	}

	while (tmp_finish == 1 && tmp_continue == 1){
		if (rand_number == RND_INTERVAL){
			printf("\n\n\nThe game is over, You are winner !!!\n");
			printf("Press Enter to back to menu\n");
			_getch();
			Write_Score(name, cnt_score);
			free(name);
			return;
		}
		// Read and save the word in the memory; print a number of letters in the word
		FILE *fl;
		int cnt = 1;
		int letters = 0;
		int tmp = 0;
		char fl_name[] = "C:/Users/Alex/Desktop/Kodding/C/Hangman/hangman_slovnik.txt";
		char c;

		char *word = malloc(sizeof(char));
		Check_Memory(word);
		*word = '\0';

		// Work with the file
		fopen_s(&fl, fl_name, "r");
		while ((c = fgetc(fl)) != EOF){
			// Find the word in the dictionary
			if (vektor_f[rand_number] == tmp){
				cnt++;
				word = realloc(word, cnt * sizeof(char));
				Check_Memory(word);
				// Waiting for the end of word and exit
				if (c == ' ' || c == '\n'){
					break;
				}
				AppendSymbol(word, c);
				letters++;
				continue;
			}
			if (c == ' '){tmp++;}
		}
		rand_number++;

		system("@cls||clear"); /*printf("Random: %d\n", rand_number); printf("\nWord: %s\n", word); printf("Letters in a word: %d\n", letters);*/
		fclose(fl);

		// Check the letter in the word and print the result
		char* correct_letter = NULL;
		correct_letter = malloc(strlen(word) + 1 * sizeof(char));
		Check_Memory(correct_letter);
		*correct_letter = '\0';

		printf("\n\n\t\t\t\t*** Hangman ***\n\n\n");
		for (int j = 0; j < strlen(word); j++){
			if (j + 1 == strlen(word) || j == 0){
				// Add first and last letter to the word
				correct_letter[j] = word[j];
			}
			else correct_letter[j] = '_';

			correct_letter[j + 1] = '\0';
			printf("\t\t %c ", correct_letter[j]);
		}
		
		int cnt_error = 5;
		char user_text[5];
		while (strcmp(word, correct_letter) != 0){
			if (cnt_error == 0){
				system("@cls||clear");
				printf("\n\t\t\t*** Game over ***\n");
				Finish();
				tmp_finish = 0;
				break;
			}
			printf("\n\nFor exit, press symbol '1'\n");
			printf("Number of life: %d\n", cnt_error);
			printf("Please, write your letter:\n");
			fgets(user_text, 5, stdin);
			fflush(stdin);

			if (user_text[1] != '\n'){ // if > 1 
				printf("\t\t\tLetter no exist in the word");
				cnt_error--;
			}
			else if (user_text[0] == '1'){
				break;
			}
			else{
				int flag_f_sym = 0;
				int correct = 1;
				for (int i = 0; i < strlen(word); i++){
					if (correct_letter[i] == user_text[0]){
						flag_f_sym = 1;
						continue;
					}
					else if (word[i] == user_text[0] && correct_letter[i] != user_text[0]){
						correct_letter[i] = user_text[0];
						correct = 0;
					}
				}
				if (correct == 1 && flag_f_sym == 0){ // No insert & no repeat
					cnt_error--;
					printf("\t\t\tLetter no exist in the word");
				}
				else if (correct == 1 && flag_f_sym == 1){ // No insert & yes repeat
					printf("\t\t\tLetter is already exist in the word");
				}
				else if (correct == 0){
					if (strcmp(word, correct_letter) == 0){ break; }
					else ReDraw(correct_letter);
				}
			}
		}

		if (cnt_error != 0){
			system("@cls||clear");
			if (user_text[0] == '1'){
				printf("\n\t\t\t*** Game over ***\n");
				Finish();
				tmp_finish = 0;
			}
			else{
				Continue_Game(&tmp_continue);
				cnt_word_score++;
				cnt_score = cnt_word_score * 3;
			}
		}
		free(correct_letter);
		free(word);
	}//printf("%d", cnt_score); // final score

	Write_Score(name, cnt_score);
	free(name);
}

// Menu
void Menu_Statistic(){
	int choose_stat = 0;
	int stat_up = 1;
	int stat_down = 1;
	int stat_all = 0;

	while (1){
		system("@cls||clear");
		printf("\n\n\n\n\t\t\t\t*** Hangman ***\n\n\n");
		printf("\t\t\t\t\tStatistic\n\n\n");
		if (stat_all == 0){
			printf("1.Back to main menu *\n");
		}
		else printf("1.Back to main menu\n");
		if (stat_all == 1 || stat_all == -1){
			printf("2.Exit *\n\n\n");
		}
		else printf("2.Exit\n\n\n");
		
		// Reading file
		FILE *f_name;
		char f_link[] = "C:/Users/Alex/Desktop/Kodding/C/Hangman/score.txt";

		fopen_s(&f_name, f_link, "r");
		fseek(f_name, 0, SEEK_END);
		int pos = ftell(f_name);

		if (pos > 0){
			rewind(f_name);
			char c;
			while ((c = fgetc(f_name)) != EOF){
				printf("%c", c);
			}
		}
		else printf("\nSorry, but the statistic doesnt exist\n");

		fclose(f_name);
		printf("\n\nChoose what do you want and press Enter.\n\n");
		printf("* - Item has been selected.\n\n\n");

		switch (choose_stat = _getch()){
		case 49:
			return;
		case 50:
			exit(0);
		case 72:
			stat_up++;
			if (stat_up > 2){
				stat_up = 1;
			}
			stat_all = stat_down - stat_up;
			break;
		case 80:
			stat_down++;
			if (stat_down > 2){
				stat_down = 1;
			}
			stat_all = stat_down - stat_up;
			break;
		case 13:
			if (stat_all == 0){
				return;
			}
			else if (stat_all == 1 || stat_all == -1){
				exit(0);
			}
		}
	}
}

void Main_Menu(){
	int start = 0;
	int cnt_down = 1;
	int cnt_up = 1;
	int cnt_all = 0;

	while (1){
		system("@cls||clear");
		printf("\n\n\n\n\t\t\t\t*** Hangman ***\n\n\n");

		if (cnt_all == 0 || cnt_all == -3){
			printf("\t\t1. Start *\n");
		}
		else printf("\t\t1. Start\n");
		
		if (cnt_all == 1 || cnt_all == -2){
			printf("\t\t2. Statistic *\n");
		}
		else printf("\t\t2. Statistic\n");

		if (cnt_all == 2 || cnt_all == -1){
			printf("\t\t3. Exit *\n\n");
		}
		else printf("\t\t3. Exit\n\n");

		printf("\t\tChoose what do you want and press Enter.\n\n");
		printf("\t\t* - Item has been selected.\n\n\n");
		printf("\n\tHangman is a game for two players. In this version you enemy is a computer.\n\tA computer generates some words and you should try to guess it by suggesting letters :)");

		switch (start = _getch()){
		case 49:
			// Game
			ReadFile();
			break;
		case 50:
			Menu_Statistic();
			break;
		case 51:
			exit(0);
		case 72:
			cnt_up++;
			if (cnt_up > 3){
				cnt_up = 1;
			}
			cnt_all = cnt_down - cnt_up;
			break;
		case 80:
			cnt_down++;
			if (cnt_down > 3){
				cnt_down = 1;
			}
			cnt_all = cnt_down - cnt_up;
			break;
		case 13:
			if (cnt_all == 0 || cnt_all == -3){
				// Game
				ReadFile();
				break;
			}
			else if (cnt_all == 1 || cnt_all == -2){
				Menu_Statistic();
				break;
			}
			else if (cnt_all == 2 || cnt_all == -1){
				exit(0);
			}
		}
	}
}


int main(int argc, char** argv){

	Main_Menu();

	_getch();
	return 0;
}