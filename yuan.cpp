#include<iostream>
#include<vector>
#include<regex>
#include<set>
#include<cstdbool>
#include<string>
#include<stack>
#include"simple_test.h"
using namespace std;


struct num_err{};
struct expr_err{};
struct truth_table_err{};

class E2T
{
private:
	string express;
	int nvar;
	vector<int> bits;
public:
	E2T(int n, const string& expr)
	{
		regex pat1("[ \t\n]");
		string exprt = regex_replace(expr, pat1, "");
		int min = 0;
		regex pata("A");
		regex patb("B");
		regex patc("C");
		regex patd("D");
		regex pate("E");
		regex patf("F");
		regex patg("G");
		regex path("H");
		if (regex_search(exprt, pata)) min = 1;
		if (regex_search(exprt, patb)) min = 2;
		if (regex_search(exprt, patc)) min = 3;
		if (regex_search(exprt, patd)) min = 4;
		if (regex_search(exprt, pate)) min = 5;
		if (regex_search(exprt, patf)) min = 6;
		if (regex_search(exprt, patg)) min = 7;
		if (regex_search(exprt, path)) min = 8;
		if (n > 8 || n < min) throw num_err{};
		regex errpat1("[^A-H()|^&~]");
		regex errpat2("\\w{2}|[|^&]{2}|~[|^&]|^[|^&]|[|^&]$|[|^&~]\\)|\\([|^&]|\\(\\)|\\)\\(|\\)~|\\w\\(|\\)\\w");
		if (exprt == "" || regex_search(exprt, errpat1) || regex_search(exprt, errpat2) || !is_bracket_valid(exprt)) throw expr_err{};
		nvar = n;
		express = exprt;
	}

	bool is_bracket_valid(const string& str)
	{
		regex patb("[^\\(\\)]");
		string str1 = regex_replace(str, patb, "");
		stack<unsigned char> stackb; 
		unsigned char chtop = ' ';  

		for (auto& ch : str1)
		{
			if (stackb.empty() && ch == ')') return false;
			else
			{
				if (!stackb.empty())
				{
					chtop = stackb.top();
					if (chtop != '(') return false;
				}

				if (chtop == '(' && ch == ')')
				{
					stackb.pop();
					chtop = ' '; 
				}
				else stackb.push(ch);
			}
		}
		if (stackb.empty()) return true;
		else return false;
	}

	string expr_to_binary()
	{
		string restr = express;
		regex pata("A");
		regex patb("B");
		regex patc("C");
		regex patd("D");
		regex pate("E");
		regex patf("F");
		regex patg("G");
		regex path("H");
		restr = regex_replace(restr, pata, to_string(bits[0]));
		restr = regex_replace(restr, patb, to_string(bits[1]));
		restr = regex_replace(restr, patc, to_string(bits[2]));
		restr = regex_replace(restr, patd, to_string(bits[3]));
		restr = regex_replace(restr, pate, to_string(bits[4]));
		restr = regex_replace(restr, patf, to_string(bits[5]));
		restr = regex_replace(restr, patg, to_string(bits[6]));
		restr = regex_replace(restr, path, to_string(bits[7]));
		return restr;
	}

	string solve_binary(const string& bistr)
	{
		string restr = bistr;
		regex pan("\\(([^\\(\\)]+)\\)");
		smatch m;
		while (regex_search(restr, m, pan))
			restr = regex_replace(restr, pan, solve_binary(m[1]), std::regex_constants::format_first_only);
		regex not0("~0");
		regex not1("~1");
		regex and00("0&0");
		regex and01("0&1");
		regex and10("1&0");
		regex and11("1&1");
		regex xor00("0\\^0");
		regex xor01("0\\^1");
		regex xor10("1\\^0");
		regex xor11("1\\^1");
		regex or00("0\\|0");
		regex or01("0\\|1");
		regex or10("1\\|0");
		regex or11("1\\|1");
		while (regex_search(restr, not0) || regex_search(restr, not1))
		{
			restr = regex_replace(restr, not0, "1");
			restr = regex_replace(restr, not1, "0");
		}
		while (regex_search(restr, and00) || regex_search(restr, and01) || regex_search(restr, and10) || regex_search(restr, and11))
		{
			restr = regex_replace(restr, and00, "0");
			restr = regex_replace(restr, and01, "0");
			restr = regex_replace(restr, and10, "0");
			restr = regex_replace(restr, and11, "1");
		}
		while (regex_search(restr, xor00) || regex_search(restr, xor01) || regex_search(restr, xor10) || regex_search(restr, xor11))
		{
			restr = regex_replace(restr, xor00, "0");
			restr = regex_replace(restr, xor01, "1");
			restr = regex_replace(restr, xor10, "1");
			restr = regex_replace(restr, xor11, "0");
		}
		while (regex_search(restr, or00) || regex_search(restr, or01) || regex_search(restr, or10) || regex_search(restr, or11))
		{
			restr = regex_replace(restr, or00, "0");
			restr = regex_replace(restr, or01, "1");
			restr = regex_replace(restr, or10, "1");
			restr = regex_replace(restr, or11, "1");
		}
		return restr;
	}

	string gen_truthtable()
	{
		bits = { 1,1,1,1,1,1,1,1 };
		int alltimes = 1 << nvar;
		string truth_table = "";
		for (int i = 0; i < alltimes; i++)
		{
			string bistr = expr_to_binary();
			string bit = solve_binary(bistr);
			truth_table += bit;
			bits[0] ^= 1;
			if (nvar >= 2 && i % 2 == 1) bits[1] ^= 1;
			if (nvar >= 3 && i % 4 == 3) bits[2] ^= 1;
			if (nvar >= 4 && i % 8 == 7) bits[3] ^= 1;
			if (nvar >= 5 && i % 16 == 15) bits[4] ^= 1;
			if (nvar >= 6 && i % 32 == 31) bits[5] ^= 1;
			if (nvar >= 7 && i % 64 == 63) bits[6] ^= 1;
			if (nvar >= 8 && i % 128 == 127) bits[7] ^= 1;
		}
		return truth_table;
	}

 };


std::string expr_to_truthtable(int n, const std::string& expr)
{
	E2T class1(n,expr);
	return class1.gen_truthtable();
}

class T2E
{
private:
	string truthtable;
	int mode = 0;
	vector<int> intminterm; 
	vector<string> binminterm; 
	int bitcount;
	int mincount;  
	vector<set<string>> table;
	vector<string> prime;
	set<int> rescomb;
public:
	int allflag = -1;

	T2E(const string& ttable)
	{
		int n = ttable.length();
		int c = count_str_one(ttable);
		regex errpat("[^01]");
		if (ttable == "" || regex_search(ttable, errpat) || (n != 2 && n != 4 && n != 8 && n != 16 && n != 32 && n != 64 && n != 128 && n != 256))
			throw truth_table_err{};
		if (c == 0)
		{
			allflag = 0;
			return;
		}
		if (c == n)
		{
			allflag = 1;
			return;
		}
		truthtable = ttable;
		reverse(truthtable.begin(), truthtable.end());
		unsigned int i, countone = 0;
		for (i = 1; i <= 8; i++) if (pow(2, i) == truthtable.length()) break;
		bitcount = i;
		for (i = 0; i < truthtable.length(); i++)
		{
			if (truthtable[i] == '1')
			{
				countone++;
				intminterm.push_back(i);
				binminterm.push_back(int_to_binstr(bitcount, i));
			}
		}
		mincount = countone;
		table = vector<set<string>>(bitcount + 1);
		for (int i = 0; i < mincount; i++)
		{
			int onecount = count_str_one(binminterm[i]);
			table[onecount].insert(binminterm[i]);
		}
	}

	int count_str_one(const string& a)
	{
		int count = 0;
		for (unsigned int i = 0; i < a.length(); i++) if (a[i] == '1') count++;
		return count;
	}

	string bin_to_expr(const string& binstr)
	{
		string restr = "";
		for (int i = binstr.length() - 1; i >= 0; i--)
		{
			int j = binstr.length() - 1 - i;
			if (binstr[i] == '1') 
			{
				char ch = 65 + j;
				restr += ch;
				restr += "&";
			}
			else if (binstr[i] == '0') 
			{
				char ch = 65 + j;
				restr += "~";
				restr += ch;
				restr += "&";
			}
		}
		if (restr[restr.length() - 1] == '&') restr.pop_back();
		return restr;
	}

	string int_to_binstr(const int& size,int a)
	{
		string binstr(size,'0');
		for (int i = 0; i < size; i++)
		{
			binstr[size - i - 1] = 48 + a % 2;
			a /= 2;
		}
		return binstr;
	}

	bool is_empty(const vector<set<string>>& table)
	{
		for (unsigned int i = 0; i < table.size(); i++)
			if (table[i].size() != 0)
				return false;
		return true;
	}

	bool is_one_diff(const string& a,const string& b)
	{
		int count = 0;
		for (unsigned int i = 0; i < a.length(); i++) if (a[i] != b[i]) count++;
		if (count == 1) return true;
		else return false;
	}

	string get_diff(string a,const string& b)
	{
		for (unsigned int i = 0; i < a.length(); i++) if (a[i] != b[i]) a[i] = '-';
		return a;
	}

	bool is_prime_icl_min(const string& p, const string& minterm)
	{
		for (unsigned int i = 0; i < p.size(); i++)
			if (p[i] != '-' && p[i] != minterm[i])
				return false;
		return true;
	}

	vector<set<string>> comb_step_1(const vector<set<string>>& table, set<string>& primeset)
	{
		vector<vector<int>> checked(table.size(), vector<int>(9999999, 0));
		vector<set<string>> newtable(table.size() - 1);
		for (unsigned int i = 0; i < table.size() - 1; i++)
		{
			set<string> ::iterator itj;
			int j = 0;
			for (itj = table[i].begin(); itj != table[i].end(); itj++)
			{
				string xj = *itj;
				set<string> ::iterator itk;
				int k = 0;
				for (itk = table[i+1].begin(); itk != table[i+1].end(); itk++)
				{
					string xk = *itk;
					if (is_one_diff(xj, xk))
					{
						newtable[i].insert(get_diff(xj, xk));
						checked[i][j] = 1;
						checked[i + 1][k] = 1;
					}
					k++;
				}
				j++;
			}
		}
		for (unsigned int i = 0; i < table.size(); i++)
			for (unsigned int j = 0; j < table[i].size(); j++)
				if (!checked[i][j])
				{
					set<string> ::iterator itk;
					int k = 0;
					for (itk = table[i].begin(); itk != table[i].end(); itk++)
					{
						string xk = *itk;
						if(k == j) primeset.insert(xk);
						k++;
					}
				}
		return newtable;
	}

	void gen_prime()
	{
		set<string> primet;
		while (!is_empty(table))
			table = comb_step_1(table, primet);
		set<string> ::iterator it;
		for (it = primet.begin(); it != primet.end(); it++)
		{
			string x = *it;
			prime.push_back(x);
		}
	}

	set<set<int>> comb_step_2(const vector<set<set<int>>>& petrick) 
	{
		set<set<int>> rescomb;
		if (petrick.size() == 1) return petrick[0];
		for (unsigned int i = 1; i < petrick.size(); i++)
		{
			if (i == 1) rescomb = mul_pet(petrick[0], petrick[1]);
			else rescomb = mul_pet(rescomb, petrick[i]);
		}
		return rescomb;
	}

	void gen_prime_comb() 
	{
		int impsize = prime.size();
		vector<vector<int>> primetable(impsize, vector<int>(mincount, 0));
		for (int i = 0; i < impsize; i++)
			for (int j = 0; j < mincount; j++)
				primetable[i][j] = is_prime_icl_min(prime[i], binminterm[j]);
		vector<set<set<int>>> petrick;
		for (int j = 0; j < mincount; ++j) 
		{
			set<int> x;
			set<set<int>> y;
			for (unsigned int i = 0; i < prime.size(); ++i) if (primetable[i][j] == true)
			{
				x.insert(i);
				y.insert(x);
			}
			petrick.push_back(y);
		}
		set<set<int>> rescombt;
		rescombt = comb_step_2(petrick);
		unsigned int min = 50000;
		set<set<int>> ::iterator it;
		for (it = rescombt.begin(); it != rescombt.end(); it++)
		{
			set<int> comb = *it;
			if (comb.size() < min)
				min = comb.size();
		}
		vector<set<int>> rescombs;
		set<set<int>> ::iterator it1;
		for (it1 = rescombt.begin(); it1 != rescombt.end(); it1++)
		{
			set<int> comb = *it1;
			if (comb.size() == min)
				rescomb = comb;
		}
	}

	string gen_expr()
	{
		string resexpr;
		set<int> ::iterator it;
		for (it = rescomb.begin(); it != rescomb.end(); it++)
		{
			int x = *it;
			resexpr += bin_to_expr(prime[x]) + "|";
		}
		resexpr.pop_back();
		return resexpr;
	}

	bool is_pet_icl(const set<int>& s1, const set<int>& s2)
	{
		set<int> ::iterator it1;
		for (it1 = s1.begin(); it1 != s1.end(); it1++)
		{
			int x1 = *it1;
			if (s2.find(x1) == s2.end()) return false;
		}
		return true;
	}

	set<set<int>> arr_pet(const set<set<int>>& p)
	{
		set<set<int>> p1 = p;
		set<set<int>> ::iterator it1;
		for (it1 = p.begin(); it1 != p.end(); it1++)
		{
			set<int> x1 = *it1;
			set<set<int>> ::iterator it2;
			for (it2 = p.begin(); it2 != p.end(); it2++)
			{
				set<int> x2 = *it2;
				if (x1 != x2 && is_pet_icl(x1, x2)) p1.erase(x2);
			}
		}
		return p1;
	}

	set<set<int>> mul_pet(const set<set<int>>& p1, const set<set<int>>& p2)
	{
		set<set<int>> p3;
		set<int> p3i;
		set<set<int>> ::iterator it1;
		for (it1 = p1.begin(); it1 != p1.end(); it1++)
		{
			set<int> x1 = *it1;
			set<set<int>> ::iterator it2;
			for (it2 = p2.begin(); it2 != p2.end(); it2++)
			{
				p3i = x1;
				set<int> x2 = *it2;
				set<int> ::iterator iti;
				for (iti = x2.begin(); iti != x2.end(); iti++)
				{
					int x = *iti;
					p3i.insert(x);
				}
				p3.insert(p3i);
			}
		}
		p3 = arr_pet(p3);
		return p3;
	}

};

std::string truthtable_to_expr(const std::string& truth_table)
{
	T2E class2(truth_table);
	if (class2.allflag == 0) return "0";
	if (class2.allflag == 1) return "1";
	class2.gen_prime();
	class2.gen_prime_comb();
	return class2.gen_expr();
}

int test_main()
{
	CHECK_THROW(expr_to_truthtable(2, "A&B|C"), num_err);
	CHECK_THROW(expr_to_truthtable(9, "A&B|C"), num_err);
	CHECK_THROW(expr_to_truthtable(1, ""), expr_err);
	CHECK_THROW(expr_to_truthtable(1, "      "), expr_err);
	CHECK_THROW(expr_to_truthtable(4, "A&b&C"), expr_err);
	CHECK_THROW(expr_to_truthtable(4, "A&B||C"), expr_err);
	CHECK_THROW(expr_to_truthtable(2, "&A|B"), expr_err);
	CHECK_THROW(expr_to_truthtable(3, "A|((B&(~C))))"), expr_err);
	CHECK_EQUAL(expr_to_truthtable(1, "A"), "10");
	CHECK_EQUAL(expr_to_truthtable(3, "~A|B&C"), "11010101");
	CHECK_EQUAL(expr_to_truthtable(3, "~A  |	B& C"), "11010101");
	CHECK_EQUAL(expr_to_truthtable(3, "~(A|B)&C"), "00010000");
	CHECK_EQUAL(expr_to_truthtable(4, "B|C"),"1111110011111100");
	CHECK_EQUAL(expr_to_truthtable(8, "A&B&C&D|B&E&G|~C&~E&F&H"), "1100110011001100100011110000111111001100110011001000000000000000100000000000000010001111000011111000000000000000100000000000000011001100110011001000000000000000110011001100110010000000000000001000000000000000100000000000000010000000000000001000000000000000");
	CHECK_THROW(truthtable_to_expr(""), truth_table_err);
	CHECK_THROW(truthtable_to_expr("   "), truth_table_err);
	CHECK_THROW(truthtable_to_expr("12100000"), truth_table_err);
	CHECK_THROW(truthtable_to_expr("111100000"), truth_table_err);
	CHECK_EQUAL(truthtable_to_expr("1111"), "1");
	CHECK_EQUAL(truthtable_to_expr("0000"), "0");
	CHECK_EQUAL(truthtable_to_expr("01"), "~A");
	CHECK_EQUAL(truthtable_to_expr("0011"), "~B");
	CHECK_EQUAL(truthtable_to_expr("11010101"), "~A|B&C");
	CHECK_EQUAL(truthtable_to_expr("0100000001000001"), "~A&B&C|~A&~B&~C&~D");
	CHECK_EQUAL(truthtable_to_expr("1100110011001100100011110000111111001100110011001000000000000000100000000000000010001111000011111000000000000000100000000000000011001100110011001000000000000000110011001100110010000000000000001000000000000000100000000000000010000000000000001000000000000000"), "A&B&C&D|B&E&G|~C&~E&F&H");
	CHECK_EQUAL(truthtable_to_expr("0101111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111"), "~A|~C|~D|~E|~F|~G|~H");
	return 0;
}




