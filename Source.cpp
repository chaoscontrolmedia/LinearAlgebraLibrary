#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>

using namespace std;



class Matrix {
	private:
		//r and c
		//Constructor
		//Declaration of member variables and functions
		int row, col;

		//Declaration of 2d matrix;
		vector<vector<double>> data;


	public:
	/*Definition:Initializer List*/
		int row_choice;
		int col_choice;

	Matrix() : row(0), col(0), data(0, vector<double>(0, 0)) {}
	Matrix(int r, int c) : row(r), col(c), data(r, vector<double>(c, 0)) {}

	friend std::ostream& operator<<(std::ostream& os, const Matrix& M);
	

	//Operators
	double& operator()(double i, double j) {
		return data[i][j];
	}

	const double& operator()(double i, double j) const {
		return data[i][j];
	}

	bool bounds_check(int row_index) const {
		return (row_index < 0 || row_index >= row);
	}

	void check_addition_dims(const Matrix& A, const Matrix& B) {
		if (A.row != B.row || A.col != B.col) {
			cout << "Addition dimension mismatch\n";
			exit(1);
		}
	}



	int getRowSize() const { return row; }
	int getColSize() const { return col; }
	//int rowChoice() const { return row_choice; }
	//int colChoice() const { return col_choice; }


	


	/*Math=============================================*/
	struct Fraction {
		double numerator;
		double denominator;

		Fraction(double num, double denom)
			: numerator(num), denominator(denom) {
		}
		Fraction() : numerator(1), denominator(0) {}


		//1/11 = {1/11}
		// 1*11 = 11 
		// 11/11 = 1
		// possibly use neg.exponents

		Fraction convertToFraction(int n) {
			return Fraction(1, n);
		}

		long long euclidean_algorithm(long long a, long long b) {
			while (b != 0) {
				long long temp = b;
				b = a % b;
				a = temp;
			}
			return a >= 0 ? a : -a;
		}



		//Fraction toFraction(double value, double epsilon = 1e-10) {
		   // long long denom = 1;
		   // while (abs(value * denom - round(value * denom)) > epsilon) {
		   //	 denom *= 10;
		   // }
		   // long long num = round(value * denom);
		   // long long gcd = euclidean_algorithm(num, denom);
		   // return Fraction(num / gcd, denom / gcd);
		//}


		Fraction toFraction(double value, double epsilon = 1e-10) {
			long long num = 1, denom = 0;
			long long prev_num = 0, prev_denom = 1;
			double x = value;

			for (int i = 0; i < 50; i++) {
				long long a = (long long)x;
				long long new_num = a * num + prev_num;
				long long new_denom = a * denom + prev_denom;

				if (abs(value - (double)new_num / new_denom) < epsilon) {
					return Fraction(new_num, new_denom);
				}

				prev_num = num; prev_denom = denom;
				num = new_num; denom = new_denom;
				x = 1.0 / (x - a);
			}
			return Fraction(num, denom);
		}



		double convertToWhole(Fraction f) {
			return f.numerator * f.denominator;
		}
		double scaleTo1(int n) {
			return 1.0 / n;
		}

	};



	////Core math
	double trace() const {
		if (row != col) {
			cout << "Trace only defined for square matrices";
			return 0;
		}

		double tr = 0;
		for (int i = 0; i < row; i++) {
			tr += (*this)(i, i);
		}
		return tr;
	}

	 double down_product(int start)const {
       int result = 1;
       cout << "\n";

       for (int i = 0; i < row; i++) {
           int j = (start + i) % col;
           result *= (*this)(i, j);
       }

       return result;
   }

	 double up_product(int start) const {
       int result = 1;
       for (int i = 0; i < row; i++) {
           int j = (start + (row - 1 - i)) % col;
           //cout << (*this)(i,j) << " ";
           result *= (*this)(i, j);
       }
       return result;
   }

	void scale_row(int row_index, double scalar) {
		if (row_index < 0 || row_index >= row) {
			cout << "Invalid row index.";
			exit(1);
		}


		for (int j = 0; j < col; j++) {
			(*this)(row_index, j) *= scalar;
		}
	}

	void scale_col(int col_index, double scalar) {
		if (col_index < 0 || col_index >= col) {
			cout << "Invalid column index.";
			exit(1);
		}
		for (int i = 0; i < row; i++) {
			(*this)(i, col_index) *= scalar;
		}
	}


	void row_add(int source, int target) {
		for (int j = 0; j < col; j++) {
			(*this)(target, j) += (*this)(source, j);
		}
	}

	void row_subtract(int source, int target) {
		for (int j = 0; j < col; j++) {
			(*this)(target, j) = (*this)(source, j) - (*this)(target, j);
		}
	}

	void reduce_row_to_1_alt(int row_index) {
		double leading_value = (*this)(row_index, 0);
		if (leading_value == 0) {
			cout << "Cannot reduce row to 1: leading value is zero.";
			return;
		}
		scale_row(row_index, 1.0 / leading_value);
	}

	void reduce_row_to_1(int row_index) {
		for(int j = 0; j<col; j++) {
			if ((*this)(row_index, j) != 0) {
				scale_row(row_index, 1.0 / (*this)(row_index, j));
				return;
			}
		}
	}

	void gauss_jordan_elimination() {
		int source, target;
		int scalar;
		int scalar2;
		//bool reduce;
		while (true) {

			cout << "Scalar and source row. 0-2 (-1 to exit):\n";
			cin >> scalar >> source;
			if (source == -1) break;
			scale_row(source, scalar);
			print_row(source);

			cout << "\nScalar and target row:\n";
			cin >> scalar2 >> target;
			if (target == -1) break;
			scale_row(target, scalar2);
			print_row(source);
			print_row(target);





			cout << "0-add, 1-Subtract, 2-Scale down to 1: ";
			int choice;
			cin >> choice;
			switch (choice) {
			case 0:
				row_add(source, target);
				break;
			case 1:
				row_subtract(source, target);
				break;
			case 2:
				reduce_row_to_1(target);
				break;
			default:
				cout << "No reduction to 1 performed.\n";
			}
			cout << "\n" << *this << "\n";
		}
	}

	Matrix transpose(){
       Matrix T(col, row);
       for (int i = 0; i< row; i++){
           for(int j = 0; j < col; j++){
               T(j,i) = (*this)(i,j); 
           }
       }
       return T;
   }

	 Matrix scalar_multiply_full_matrix(double scalar) const {
       Matrix S(row, col);
       for (int i = 0; i < row; i++) {
           for (int j = 0; j < col; j++) {
               S(i, j) = (*this)(i, j) * scalar;
           }
       }
       return S;
   }

	 //Gives me a vector object to modify
	 Matrix vector_matrix(int col_index) const {
		 if (col_index < 0 || col_index >= col) {
			 cout << "Invalid column index.";
			 exit(1);
		 }
		 Matrix V(row, 1);
		 for (int i = 0; i < row; i++) {
			 V(i, 0) = (*this)(i, col_index);
		 }
		 return V;
	 }

	 Matrix row_matrix(int row_index) const {
		 if (row_index < 0 || row_index >= row) {
			 cout << "Invalid row index.";
			 exit(1);
		 }
		 Matrix R(1, col);
		 for (int j = 0; j < col; j++) {
			 R(0, j) = (*this)(row_index, j);
		 }
		 return R;
	 }

	 static Matrix add(const Matrix& A, const Matrix& B) {
		 cout << "Matrix A:\n";A.print_full_matrix();
		 cout << "Matrix B:\n"; B.print_full_matrix();
		 if (A.getRowSize() != B.getRowSize() || A.getColSize() != B.getColSize()) {
			 cout << "Matrices cannot be added\n";
			 exit(1);
		 }
		 Matrix C(A.getRowSize(), A.getColSize());
		 for (int i = 0; i < A.getRowSize(); i++) {
			 for (int j = 0; j < A.getColSize(); j++) {
				 C.data[i][j] = A.data[i][j] + B.data[i][j];
			 }
		 }
		 return C;
	 }

	 static Matrix multiply(const Matrix& A, const Matrix& B) {
		 if (A.col != B.row) {
			 cout << "Matrices cannot be multiplied\n";
			 exit(1);
		 }
		 Matrix C(A.row, B.col);
		 for (int i = 0; i < A.row; i++) {
			 for (int j = 0; j < B.col; j++) {
				 C(i, j) = 0;
				 for (int k = 0; k < A.col; k++) {
					 C(i, j) += A(i, k) * B(k, j);
				 }
			 }
		 }

		 return C;
	 }

	 struct Det2Result {
		 double down;
		 double up;
		 double value;
	 };

	 Det2Result determinant_2dim() const {
		 double down = (*this)(0, 0) * (*this)(1, 1);
		 double up = (*this)(0, 1) * (*this)(1, 0);

		 return { down, up, down - up };
	 }

	 void determinant2d_info(Matrix& M) {
		 cout << "\nDown product starting at 0 = " << M.down_product(0);
		 cout << "\nUp product starting at 0 = " << M.up_product(0);
		 auto d = M.determinant_2dim();
		 cout << "\nDeterminant" << d.down << " - " << d.up << " = " << d.value;
	 }

	 struct Det3Result {
		 double down_diagonals[3];
		 double up_diagonals[3];
		 double down_sum;
		 double up_sum;
		 double value;

	 };

	 Det3Result determinant_3dim() const {
		 // pure computation, no cout
		 Det3Result result;
		 result.down_sum = 0;
		 result.up_sum = 0;
		 for (int i = 0; i < 3; i++) {
			 result.down_diagonals[i] = down_product(i);
			 result.down_sum += result.down_diagonals[i];
			 result.up_diagonals[i] = up_product(i);
			 result.up_sum += result.up_diagonals[i];
		 }
		 result.value = result.down_sum - result.up_sum;
		 return result;
	 }



	    int determinant_3dim_info() {
        if (row != 3 || col != 3) {
            cout << "Matrix must be 3^2 size.";
            exit(1);
        }

    	int down_sum = 0;
    	int up_sum = 0;

        cout << "\n";


        for (int i = 0; i < row; i++) {
            cout << "\nDiagonal " << i << " : ";
    		print_down_diagonal(i);

            int d = down_product(i);
    		down_sum += d;
    		cout << "Product: " << d << "\n";
    		cout << "Down sum so far: " << down_sum << "\n";

        }
        for (int i = 0; i < row; i++) {
            cout << "\nDiagonal " << i << " : ";
            print_up_diagonal(i);

            int u = up_product(i);
            up_sum += u;
            cout << "Product: " << u    << "\n";
            cout << "Up sum so far: " << up_sum << "\n";

        }

        cout << "\nDownward diagonal sum: " << down_sum << "\n";
        cout << "\nUpward diagonal sum: " << up_sum << "\n";

        cout << "\nDeterminant = ";


        return (down_sum - up_sum);

    }









	/*===========================================Print functions=======================================================================*/

	 //Member function. Must be called on an instance of the Matrix class. Prints the specified row of the matrix.
	 void print_row(int row_index) const {
		 if (bounds_check(row_index)) {
			 cout << "Invalid row index.";
			 exit(1);
		 }

		 for (int j = 0; j < col; j++) {
			 cout << (*this)(row_index, j) << ' ';
		 }
		 cout << '\n';
	 }


	 void print_vector(int col_index) const {
		 if (bounds_check(col_index)) {
			 cout << "Invalid column index.";
			 exit(1);
		 }
		 for (int i = 0; i < row; i++) {
			 cout << data[i][col_index] << ' ';
		 }
		 cout << '\n';
	 }


	 void print_down_diagonal(int start) const {
		 for (int i = 0; i < row; i++) {
			 int j = (start + i) % col;
			 cout << (*this)(i, j) << ' ';
		 }
		 cout << "\n";
	 }

	 void print_up_diagonal(int start) const {
		 for (int i = row - 1; i >= 0; i--) {
			 int j = (start + (row - 1 - i)) % col;
			 cout << (*this)(i, j) << ' ';
		 }
		 cout << "\n";
	 }

	 void print_full_matrix() const {
		 for (int i = 0; i < row; i++) {
			 for (int j = 0; j < col; j++) {
				 cout << (*this)(i, j) << ' ';
			 }
			 cout << '\n';
		 }
	 }




	 ////Belongs to the Matrix class.
	 void user_populate() {
		 
		 for (int i = 0; i < row; i++) {
			 for (int j = 0; j < col; j++) {
				 cout << "Enter element [ " << i << "][" << j << "]: ";
				 cin >> (*this)(i, j);
			 }
		 }
	 }

	 void random_populate(int random_limit) {
		 for (int i = 0; i < row; i++) {
			 for (int j = 0; j < col; j++) {
				 (*this)(i, j) = rand() % random_limit;
			 }
		 }
	 }

	 void writeMatrixToFile(const string& filename) const {
		 ofstream outFile(filename);
		 if (!outFile) {
			 cout << "Error opening file for writing: " << filename << "\n";
			 return;
		 }
		 for (int i = 0; i < row; i++) {
			 for (int j = 0; j < col; j++) {
				 outFile << (*this)(i, j) << ' ';
			 }
			 outFile << '\n';
		 }
		 outFile.close();
	 }

	 void readMatrixFromFile(const string& filename) {
		 ifstream inFile(filename);
		 if (!inFile) {
			 cout << "Error opening file for reading: " << filename << "\n";
			 return;
		 }
		 for (int i = 0; i < row; i++) {
			 for (int j = 0; j < col; j++) {
				 inFile >> (*this)(i, j);
			 }
		 }
		 inFile.close();
	 }









	 void linear_independence(double determinant) {
		 cout << "This matrix is ";
		 if (determinant != 0) {
			 cout << "linearly independent.\n";
		 }
		 else {
			 cout << "linearly dependent.\n";
		 }
		 return;
	 }
	 void test() {

		 print_vector(0);

	 }


	 //Main menu functions




	 //Matrix I/O

	 //Main menu functions
//Level 0 functions that return a Matrix object. 
// These will be called in the main menu

	 static Matrix create_user_matrix() {
		 int cols = 0;
		 int rows = 0;
		 //int choice;

		 cout << "Populate matrix\n";
		 cout << "Cols:";
		 cin >> cols;
		 cout << "\nRows:";
		 cin >> rows;
		 Matrix M(rows, cols);
		 M.user_populate();
		 cout << "\nResulting Matrix:\n";
		 return M;
	 }

	 static void save_matrix(Matrix& M) {
		 string filename;
		 int filetype;



		 M.print_full_matrix();
		 cout << "Filename: ";
		 cin >> filename;
		 /* may add support for different file types and writing to difff. directories*/
		 cout << "Filetype:\n";
		 cin >> filetype;
		 switch (filetype) {
		 case 0:
		 {
			 string txtFile = filename + ".txt";
			 cout << "Saving as .txt. \n";
			 M.writeMatrixToFile(txtFile);
			 break;
		 }
		 case 1:
		 {
			 string csvFile = filename + ".csv";
			 cout << "Saving as .csv. \n";
			 M.writeMatrixToFile(csvFile);
			 break;
		 }
		 case 2:
		 {
			 string jsonFile = filename + ".json";
			 cout << "Saving as .json. \n";
			 M.writeMatrixToFile(jsonFile);
			 break;

		 }
		 default:
		 {
			 cout << "Invalid file type choice. Defaulting to .txt\n";
			 string txtFile = filename + ".txt";
			 M.writeMatrixToFile(txtFile);
			 break;
		 }
		 }
	 }


	 static Matrix load_matrix() {
		 string filename;
		 int filetype;
		 cout << "Filename: ";
		 cin >> filename;
		 Matrix M;
		 cout << "Filetype:\n";
		 cin >> filetype;
		 switch (filetype) {
		 case 0:
		 {
			 cout << "Loading from .txt. \n";
			 M.readMatrixFromFile(filename + ".txt");
			 break;
		 }
		 case 1:
		 {
			 cout << "Loading from .csv. \n";
			 M.readMatrixFromFile(filename + ".csv");
			 break;
		 }
		 case 2:
		 {
			 cout << "Loading from .json. \n";
			 M.readMatrixFromFile(filename + ".json");
			 break;
		 }
		 default:
		 {
			 cout << "Invalid file type choice. Defaulting to .txt\n";
			 M.readMatrixFromFile(filename + ".txt");
			 break;
		 }
		 }
		 return M;
	 }


	 static Matrix update_matrix(Matrix& M) {
		 int col_index;
		 int row_index;
		 cout << "Choose col and row";
		 cin >> col_index >> row_index;
		 cout << "Current value: " << M(row_index, col_index) << "\n";

		 //Replace value at col and row index with user input
		 double new_value;
		 cout << "Enter new value: ";
		 cin >> new_value;
		 M(row_index, col_index) = new_value;
	 }




	 //Random generation of a matrix. User specifies dimensions and random limit.
	 // Matrix is populated with random integers from 0 to random_limit-1.
	 static Matrix generate_random_matrix() {
		 int cols = 0;
		 int rows = 0;
		 int random_limit = 0;
		 cout << "Cols? ";
		 cin >> cols;
		 cout << "Rows? ";
		 cin >> rows;
		 Matrix M(rows, cols);
		 cout << "Random limit: ";
		 cin >> random_limit;
		 cout << "\nResulting Matrix: \n";
		 M.random_populate(random_limit);
		 return M;
	 }










};

/*====================Menu Utilities===============================*/










//A function for how the user generates the matrix.
//They can choose to load a matrix from a file, 
// create a matrix by inputting values, 
// or generate a random matrix.
// The generated or loaded matrix is printed to the console.
void generate_matrix(Matrix& M) {
	int gen_choice;
	int load_choice;
	int update_choice;
	cout << "Load matrix? Yes(0) | No(1)\n";
	cin >> load_choice;
	if(load_choice == 0){
		M =Matrix::load_matrix();
		M.print_full_matrix();
		return;
	}

	cout << "Choose matrix generation method:\n";
	cout << "User(0) | Random(1) | Save(2)\n";
	cin >> gen_choice;

	if (gen_choice == 0) {
		M =Matrix::create_user_matrix();
		M.print_full_matrix();
	}

	if (gen_choice == 1) {
		M=Matrix::generate_random_matrix();
		M.print_full_matrix();
	}
	if(gen_choice == 2){
		Matrix::save_matrix(M);
	}

	cout << "Update matrix? Yes(0) | No(1)\n";
	cin >> update_choice;
	if (update_choice == 0) {
		Matrix::update_matrix(M);
		M.print_full_matrix();
	}

}




/*======================*Subprograms==============================*/
void row_menu(Matrix& M) {
	vector<int> row_indices;



	cout << "Choose row number: ";
	
	cin >> M.row_choice;
	cout << "->r" << M.row_choice << "=";
	M.print_row(M.row_choice);
	cout << "\nRS = span{";
	for (int i = 0; i < M.getRowSize(); i++) {
		if (i < M.getRowSize() - 1) cout << "->r" << i << ",";
	}
	cout << "}\n=";

	for (int i = 0; i < M.getRowSize(); i++) {
		M.print_row(i);
		if (i < M.getRowSize() - 1) cout << ", ";
	}
	
	
	//for (int i = 1; i < M.getRowSize(); i++) {
	//	cout << "" << "->r" << i << ",";
	//}

	



}
void col_menu(Matrix& M) {
	cout << "Choose col number: ";
	cin >> M.col_choice;
	cout << "c  " << M.col_choice;
	M.print_row(M.col_choice);
	
}






//Matrix passed by reference	
void all_prints(Matrix&M) {
	int print_choice;
	
	int col_choice;
	int down_diag_choice;
	int up_diag_choice;
	

	cout << "Prints: \n";
	cout << "Row Menu(0) | Column Menu (1)| Print down diagonal(2) | Print up diagonal(3)\n";
	cin >> print_choice;
	switch (print_choice) {
		case 0: {
			row_menu(M);
			break;
		}
		case 1: {
			col_menu(M);
			break;
		}
		case 2: {
			cout << "Down Diagonal: ";
			cin >> down_diag_choice;
			M.print_down_diagonal(down_diag_choice);
			break;
		}
		case 3: {
			cout << "Up Diagonal: " << print_choice;
			cin >> up_diag_choice;
			M.print_up_diagonal(up_diag_choice);
			break;
		}
	}
}
void all_scalars(Matrix& M) {
	int scale_choice;
	cout << "Scalars:\n";
	cout << "Scale full matrix\n";
	cout << "Scale column\n";
	cout << "Scale row\n";
	cout << "Scalar multiply full matrix\n";
	cout << "Vector-matrix (get column as vector)\n";
	cout << "Row-matrix (get row as vector)\n";

	cin >> scale_choice;

	switch (scale_choice) {
		case 0: {
			M = M.scalar_multiply_full_matrix(2);
			M.print_full_matrix();
			break;
			}
		case 1: {
			M.scale_col(scale_choice, 2);
			M.print_full_matrix();
			break;
		}
		case 2: {
			M.scale_row(0, scale_choice);
			M.print_full_matrix();
			break;
		}
		case 3: {
			M = M.scalar_multiply_full_matrix(scale_choice);
			M.print_full_matrix();
			break;
		}
		case 4: {
  			M = M.vector_matrix(scale_choice);
  			M.print_full_matrix();
  			break;
		}
		case 5: {
  			M = M.row_matrix(scale_choice);
  			M.print_full_matrix();
  			break;
		}
	}
}
void all_operations(Matrix& M) {
	int operations_choice;
	int source_row;
	int target_row;
	cout << "Operations:\n";
	cout << "Add row to another row(0)\n";
	cout << "Subtract row from another row(1)\n";
	cout << "Reduce row to 1(2)\n";
	cout << "Transpose matrix(3)\n";
	cout << "Gauss Jordan Elimination(4);";
	cout << "Add to another matrix(5)";
	cout << "Multiply to another matrix(6)";
	cout << "Trace(7)";

	cin >> operations_choice;

	switch (operations_choice) {
		case 0: {
			cout << "Choose source";
			cin >> source_row;
			cout << "target row";
			cin >> target_row;
			M.row_add(source_row, target_row);
			M.print_full_matrix();
			break;
		}

		case 1: {
			cout << "Choose source";
			cin >> source_row;
			cout << "target row";
			cin >> target_row;
			M.row_subtract(source_row, target_row);
			M.print_full_matrix();
			break;
		}

		case 2: {
			M.reduce_row_to_1(0);
			M.print_full_matrix();
			break;
		}

		case 3: {
			auto MT = M.transpose();
			cout << "Transposed:" << MT;
			MT.print_full_matrix();
			break;
		}
		case 4: {
			Matrix N;
			generate_matrix(N);
			cout << "2nd matrix";
			auto MN =M.add(M,N);
			cout << "Result:";
			cout << MN;
			break;
		}
		case 5:{
			Matrix E;
			E.gauss_jordan_elimination();
			E.print_full_matrix();
		}
		case 6: {
			Matrix P;
			generate_matrix(P);
			cout << "2nd Matrix";
			auto MP = M.multiply(M, P);
			cout << "Result:";
			cout << MP;
			break;
		}
		case 7: {
			cout << "\nTrace = " << M.trace();
		}

	}
	
}


//void get_determinants(Matrix& M) {
//	if (M.getRow() == 2 && M.getCol() == 2) {
//		M.matrix_info_2d();
//	}
//	if (M.getRow() == 3 && M.getCol() == 3) {
//		M.determinant_3dim();
//	}
//	if (M.getRow() > 3 && M.getCol() > 3){
//		cout << "NxN version soon";
//	}
//
//}







void main_menu() {
	int info_choice;
	
	Matrix M;

	cout << "Chaos Control Media-Linear Algebra Program\n";
	cout << "Accept the Darkness.\n\n";
	generate_matrix(M);
	while(true){ 
			cout << "\nShow all prints(0)\n";
			cout << "Show all scalars(1)\n";
			cout << "Show all operations(2)\n";
			cout << "Get determinants(3)\n";
			cin >> info_choice;
	
			switch (info_choice) {
			case 0: {
				all_prints(M);
				break;
				}

			case 1: {
				all_scalars(M);
				break;
				}

			case 2: {
				all_operations(M);
				break;
			}
			case 3: {
				//get_determinants(M);
				break;
			}
		}
	}






	
	




	
	



	

	


}




/*========================Overloaded Ops===========================*/
ostream& operator<<(std::ostream& os, const Matrix& M) {
	for (int i = 0; i < M.row; i++) {
		for (int j = 0; j < M.col; j++) {
			os << M(i, j) << " ";
		}
		os << '\n';
	}
	return os;
}

ostream& operator<<(ostream& os, const Matrix::Det3Result& d) {
	os << "Determinant: " << d.down_sum << " - " << d.up_sum << " = " << d.value << "\n";
	return os;
}


ostream& operator<< (ostream& os, const Matrix::Fraction& f) {
	return os << f.numerator << "/" << f.denominator;
}


int main() {
	//Matrix E(2, 2);
	//cout << "Matrix must be 2^2 size.";
	//E(0, 0) = 2;
	//E(0, 1) = 3;
	//E(1, 0) = 4;
	//E(1, 1) = 5;
	//E.test();
	//E.print_full_matrix();
	//cout << "Row Sixe:" << E.getRowSize();

	main_menu();

	//Matrix M2(2, 1);
	//M2.test();


	//cout << M2;
	//cout << "\n";

	//Matrix M(2, 2);
	//M(0, 0) = 2;
	//M(0, 1) = 5;
	//M(1, 0) = 1;
	//M(1, 1) = 2;
	//cout << M;
	//cout << "\n";


	
		//cout << M5;
	//M5.row_reduction();



	








	//M.row_reduction();

	Matrix::Fraction f2;
	//f2.convertToFraction(14);

	//cout << f2 << "\n";
	Matrix::Fraction f3(1 , 11);


	


	//cout <<f3.convertToWhole(f3);


	//cout << f3 << "\n";

	//volatile double v1 = 1.0;
	//volatile double v2 = 3.0;
	//volatile double result = v1 / v2;

	//cout << result << "\n";
	//cout << result * 2 << "\n";
	//cout << fixed <<setprecision(20) << (result * v2) << "\n";

	//if(result * v2 == 1.0){
	//	cout << "Equal\n";
	//} else {
	//	cout << "Not equal\n";
	//}


	//cout << setprecision(20) << 0.3333333333333 * 3;

	



	/*Matrix::Fraction::convert(14);*/

	

	

	
	//auto det3 = M2.determinant_3dim();
	//cout << det3;

	//matrix_info_2D();

	//M.determinant_3dim();

	//auto MT = M.transpose();
	//cout <<"Transposed:" << MT;

	//M.row_view(0)[1] = 10;
	return 0;	
}




















