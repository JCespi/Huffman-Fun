#include <stdio.h>
#include <stdlib.h>
#include "table.h"

//Macros
#define TOP_BAR 1
#define MID_BAR 2
#define BOT_BAR 3
#define FAILURE 0
#define SUCCESS 1

//Global variables
FILE *out_ptr = NULL;	 //where to direct stdout
unsigned n_cols = 0; //the number of columns
unsigned *col_lens;  //array containing column widths

void set_dump_file(FILE *fp){
	out_ptr = fp;
}

void set_col_lens(unsigned *arr, unsigned n){
	col_lens = arr;
	n_cols = n;
}

unsigned total_row_len(void){
	unsigned total_len, i;

	total_len = 0;

	if (!n_cols || !col_lens)
		return total_len;
	
	for (i=0; i < n_cols; i++){
		total_len += col_lens[i];
	}

	return total_len;
}

void print_pretty_bar(int position){
	unsigned s, l, div_index, total_len;
	char *l_ch, *r_ch, *div_ch, *hor_ch;
		
	//stop if one of the globals not defined
	if (!out_ptr || !n_cols || !col_lens)
		return;

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
	fprintf(out_ptr, "%s", l_ch);

	//helps with marking the dividing chars
	l = 0;
	total_len = total_row_len();
	div_index = col_lens[l++];

	//print either div char or horizontal char
	for (s=0; s < total_len; s++){
		fprintf(out_ptr, "%s", (s == div_index) ? div_ch : hor_ch);

		if (s == div_index)
			div_index += col_lens[l++];
	}
			
	//last char and newline
	fprintf(out_ptr, "%s\n", r_ch);
}

void pretty_row_helper(unsigned col_len, char *str){
	unsigned l_pad, r_pad, i, entry_len;

	entry_len = strlen(str);

	//impossible to fit the given string in the column
	if (entry_len > col_len)
		return;

	//compute the padding to put on either side of string
	l_pad = (col_len - entry_len) / 2;
	r_pad = col_len - (l_pad + entry_len);

	//left padding
	for (i=0; i < l_pad; i++)
		fprintf(out_ptr, " ");

	//the string itself
	fprintf(out_ptr, "%s", str);

	//right padding
	for (i=l_pad+entry_len; i < r_pad; i++)
		fprintf(out_ptr, " ");	
}

unsigned n_digits(unsigned num){
	unsigned count;

	for (count = 0; num != 0; count++)
		num = num / 10;
	
	return count;
}

int print_pretty_row(char **col_names){
	unsigned i, col_len, entry_len;
	char *vert_ch;

	if (!out_ptr || !col_names)
		return FAILURE;

	vert_ch = "║";

	for (i=0; i < col_lens; i++){
		fprintf(out_ptr, "%s", vert_ch);
		pretty_row_helper(col_lens[i], col_names[i]);
	}

	fprintf(out_ptr, "%s\n", vert_ch);

	return SUCCESS;	
}

int print_pretty_header(char **col_names){

	if (!out_ptr || !col_names)
		return FAILURE;

	//decorative bars surround the row of strs
	print_pretty_bar(TOP_BAR);
	print_pretty_row(col_names);
	print_pretty_bar(BOT_BAR);

	return SUCCESS;
}

int print_pretty_footer(){
	if (!out_ptr)
		return FAILURE;
	print_pretty_bar(BOT_BAR);
	return SUCCESS;
}