//given a FILE*, writes table to that file.
void set_dump_file(FILE *fp);

//given an array of column lengths and n_cols, saves info for future functions
void set_col_lens(unsigned *col_lens, unsigned n_cols);

//given a character returns string. user's responsibility to free
char *convert_letter_to_str(char ch);

//given a decimal number and a num of bits, returns (str) binary number equivalent
char *convert_dec_to_bin(unsigned decimal_n, unsigned n_bits);

//given an integer, returns the number of digits in that number
unsigned get_n_digits(unsigned num);

//returns 1 on success. 0 on failure
int print_pretty_header(char **col_names);

//returns 1 on success. 0 on failure
int print_pretty_row(char **col_names, int separator);  

//returns 1 on success. 0 on failure
int print_pretty_footer();

//prints outside of the table but centered w/respect to the table
int print_pretty_centered(char *str);
