#include <iostream>
using namespace std;

/*Linear Independence
* 


*/









void Theorem_6_proof() {
	cout << "Required!\n";
	cout << "Suppose S = {V1..Vn}.\nThen S is linearly independent iff the det(A) = [V1..Vn] is nonzero.";
	cout << "Let V1 = (V11,...,V1n)\n";
	cout << "Let Vn = (Vn1,...,Vnn)\n";
	cout << "=> {K_11 V_11 + ... + Kn_Vn1 = 0}   \n";
	cout << ".......\n";
	cout << "{K_1V1n + ...+KnVnn = 0\n";

	cout << "Let A = {V11...V1n}\n";
	cout << ".......\n";
	cout << "{Vn1...Vnn}\n";
	cout << "= V1...Vn\n";

	cout << "k-> = [k1....kn] => A^Tk-> = 0->\n";
	cout << "=> det(A) = det(A^T) != 0\n";
	cout << "S is lin.independent<=> det(A) != 0\n";
}
//int main() {
//	Theorem_6_proof();
//	return 0;
//}