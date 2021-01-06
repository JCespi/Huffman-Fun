#include <stdio.h>
#include <stdlib.h>
#include "table.h"

//given a decimal number and a num of bits, returns (str) binary number equivalent
char *convert_dec_to_bin(unsigned decimal_n, unsigned n_bits){
	int i;
	unsigned bin_num[n_bits], j;
	char *str_bin_num;

	str_bin_num = calloc(n_bits + 1, sizeof(char));

	//fill in the integer array "backwards"
	for (i=0; decimal_n > 0; i++, decimal_n /= 2)
		bin_num[i] = decimal_n % 2;
	
	//fill the rest of the array with 0's
	for (; i < n_bits; i++)
		bin_num[i] = ZERO_BIT;
	
	//fill the string representation
	for (i=n_bits-1, j=0; i >= 0; i--, j++)
		str_bin_num[j] = (bin_num[i]) ? '1' : '0';
	
	str_bin_num[n_bits] = '\0';
	
	return str_bin_num;
}

void print_pretty_bar(FILE *fp, unsigned *lens, int n_lens, int position){
	unsigned i, j, tot_hor_len;
	char *l_ch, *r_ch, *div_ch, *hor_ch;
		
	//assign appropriate box characters
	hor_ch = "═";
	if (position == TOP_BAR){
		l_ch   = "╔";
		r_ch   = "╗";
		div_ch = "╦";
	} else if (position == MID_BAR){
		l_ch   = "╠";
		r_ch   = "╣";
		div_ch = "╬";
	} else if (position == BOT_BAR){
		l_ch   = "╚";
		r_ch   = "╝";
		div_ch = "╩";
	} else
		return;

	//first char
	fprintf(fp, "%s", l_ch);

	//add marks at each of the specified points (cumulative)
	tot_hor_len = lens[n_lens - 1];

	for (i=0; i < tot_hor_len; i++)
		for (j=0; j < n_lens; j++){
			if (i == lens[j]){
				fprintf(fp, "%s", div_ch);
				break;
			} else
				fprintf(fp, "%s", hor_ch);
		}	

	//last char
	fprintf(fp, "%s\n", r_ch);
}

void pretty_row_helper_str(FILE *fp, unsigned col_len, unsigned entry_len, char *str){
	unsigned l_pad, r_pad, i;

	l_pad = col_len / entry_len;
	r_pad = col_len - (l_pad + entry_len);

	for (i=0; i < l_pad; i++)
		fprintf(fp, " ");
	fprintf(fp, "%s", str);
	for (i=l_pad+entry_len; i < col_len; i++)
		fprintf(fp, " ");	
}

void pretty_row_helper_int(FILE *fp, unsigned col_len, unsigned entry_len, unsigned data){
	unsigned l_pad, r_pad, i;

	l_pad = col_len / entry_len;
	r_pad = col_len - (l_pad + entry_len);

	for (i=0; i < l_pad; i++)
		fprintf(fp, " ");
	fprintf(fp, "%d", data);
	for (i=l_pad+entry_len; i < col_len; i++)
		fprintf(fp, " ");
}

unsigned n_digits(unsigned num){
	unsigned count;

	for (count = 0; num != 0; count++)
		num = num / 10;
	
	return count;
}

void print_pretty_row(FILE *fp, unsigned *lens, int n_lens, unsigned *data){
	unsigned i, col_len, entry_len;
	char *vert_ch, *str_bin_num;

	vert_ch = "║";

	for (i=0; i < lens[n_lens - 1]; i++){
		fprintf(fp, "%s", vert_ch);
		col_len = lens[i];

		if (i != 2){
			entry_len = n_digits(data[i]);
			pretty_row_helper_int(fp, col_len, entry_len, data[i]);
		} else {
			entry_len = data[2];
			str_bin_num = convert_dec_to_bin(data[2], data[3]);
			pretty_row_helper_str(fp, col_len, entry_len, str_bin_num);
			free(str_bin_num);
		}

	}
	fprintf(fp, "%s\n", vert_ch);	
}

void print_pretty_title(FILE *fp, unsigned *section_lens){
	int i;
	char **titles;

	titles = malloc(sizeof(char*) * N_INFO_SECTS);
	titles[0] = strdup("Letter");
	titles[1] = strdup("Freq");
	titles[2] = strdup("Codeword");
	titles[3] = strdup("N_bits");

	//print to the file pointer
	for (i=0; i < N_INFO_SECTS; i++){
		fprintf(fp, "║");
		pretty_row_helper_str(fp, section_lens[0], strlen(titles[i]), titles[i]);
	}

	fprintf(fp, "║\n");

	//free the string array
	for (i=0; i < N_INFO_SECTS; i++)
		free(titles[i]);
	free(titles);
}

//dumps letter|frequency|code(2)|code(10)|n_bits to a file
void dump_input_info(Code_Word *codewords, unsigned *freq_table){
	unsigned code, n_bits, i, *a_section_lens, *section_lens, acc_len, data[N_INFO_SECTS];
	FILE *fp;

	//open the file to dump tree into
	fp = fopen("info", "w");

	//create section lengths
	section_lens   = calloc(N_INFO_SECTS, sizeof(int));
	a_section_lens = calloc(N_INFO_SECTS, sizeof(int));
	section_lens[0] = a_section_lens[0] = 5;
	section_lens[1] = a_section_lens[1] = 5;
	section_lens[2] = a_section_lens[2] = 10;
	section_lens[3] = a_section_lens[3] = 5;

	//turn the array into an cumulative array
	for (i=0, acc_len=0; i < N_INFO_SECTS; i++){
		acc_len += a_section_lens[i];
		a_section_lens[i] = acc_len;
	}

	//print the average length
	fprintf(fp, "Average Length of codewords = %0.2f\n", find_avg_len(codewords));

	//print the column strings
	print_pretty_bar(fp, a_section_lens, N_INFO_SECTS, TOP_BAR);
	print_pretty_title(fp, section_lens);
	print_pretty_bar(fp, a_section_lens, N_INFO_SECTS, MID_BAR);

	//print the rows
	for (i=0; i < N_CHARS; i++){
		if (code_exists(codewords[i])){
			n_bits = codewords[i].n_bits;
			code = codewords[i].code_d;
			
			data[0] = i; data[1] = freq_table[i]; data[2] = code; data[3] = n_bits;
			print_pretty_row(fp, section_lens, N_INFO_SECTS, data);
			print_pretty_bar(fp, a_section_lens, N_INFO_SECTS, MID_BAR);
		}
	}
	print_pretty_bar(fp, a_section_lens, N_INFO_SECTS, BOT_BAR);
	
	free(section_lens);
	free(a_section_lens);
	fclose(fp);
}
