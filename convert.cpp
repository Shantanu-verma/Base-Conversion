
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <cassert>

class string_int{
public:
	std::string digits;
	typedef std::string::const_iterator const_iterator;
	size_t _size;
	enum base_type{
		BIN = 2,
		DEC = 10,
		OCT = 8,
		HEX = 16,
		UNK = 0
	};
	base_type type;
	string_int():_size{ 0 } { }
	string_int(const std::string& num) : _size{ num.size() }, digits{ num } , type{ get_base() } { }
	string_int(const string_int& rhs)  : string_int(rhs.digits) { }
	string_int(const string_int&& rhs) : string_int(rhs.digits) { }

	const char* c_str() const noexcept{ return digits.c_str(); }
	const size_t size() const noexcept{ return _size; }
	const std::string& string_data() const noexcept{ return digits; }
	char &operator[](const size_t& n){
		return digits[n];
	}
	string_int &operator = (const string_int& rhs){
		this->digits = rhs.string_data();
		this->_size = rhs.size();
		return *this;
	}
	string_int &operator = (const std::string& rhs){
		this->digits = rhs;
		this->_size = rhs.size();
		return *this;
	}
	const_iterator begin() const{
		return digits.begin();
	}
	const_iterator end() const{
		return digits.end();
	}
	// My school Method
	friend std::string operator/(const string_int& lhs,const unsigned int& n){
		int num(0);
		std::string questiont;
		for(const auto& i:lhs){
			num = ( ( num % n ) * 10) + (i-'0') ;
			questiont += (num/n + '0');
		}
		return questiont;
	}

	friend int operator% (const string_int& lhs,const unsigned int& n) {
		int num = 0,rem = 0;
		for(const auto& i:lhs){
			num =  ( ( rem * 10) + (i-'0') );
			rem = num % n;
		}
		return rem;
	}
	friend std::string operator +(const std::string& data1,const std::string& data2){
		std::string big = data1.size()>=data2.size()?data1:data2;
		std::string small = data1.size()<=data2.size()?data1:data2;
		std::string number;
	
		int carry = 0 , plus = 0 , num = 0 , j = small.size() - 1;
		for(int i = big.size()-1 ; i >=0 ; --i){
			plus  = carry + (big[i] - '0') + (j>=0?small[j]-'0':0);
			carry = plus / 10;
			num = plus % 10;
			number += (num + '0');
			--j;
		}
		return number;

	}
	bool allow_processing_via_rules(){
		for(const auto& i:digits){
			if( i>='1' && i<='9' || i>='A' && i<='F' || i>='a' && i<='f')
			 return true;
		}
		return false;
	}

	const base_type get_base() const noexcept{

		if(digits[0] == '0' && digits[1] == 'x')      return HEX;
		else if(digits[0] == '0' && digits[1] == 'b') return BIN;
		else if(digits[0] == '0')                     return OCT;
		else if(digits[0]>'0' && digits[0] <='9')     return DEC;
		else                                          return UNK;
	}
	// Get string with removed prefix eg:0x , 0b , 0
	std::string get_rem_base_terms() const{

		base_type btype = get_base();
		int remove_c = 0;
		if(btype == BIN || btype == HEX)
			remove_c = 2;
		else if(btype == OCT) 
			remove_c = 1;
		std::string temp = digits.substr(remove_c,_size);
		return temp;
	}
	// Check string for invalid chars
	bool string_int_isvalid() const noexcept{

		std::string temp = get_rem_base_terms();
		base_type btype = get_base();
		if( btype != BIN && btype != OCT && btype != HEX && btype != DEC)
			 return false;
		for(const auto& i:temp){
			switch(btype){
				case BIN : if(i!='0' && i!='1') return false;
				case OCT : if(i<'0' || i>'7') return false;
				case DEC : if(i<'0' || i>'9') return false;
				case HEX : if((i<'0' || i>'9') && (i<'A' || i>'F') && (i<'a' || i>'f')) return false;
			}
		}
		return true;
	}
};
class conversions:protected string_int{
enum base_size{
	MAX_DEC = 19,
	MAX_BIN = 63,
	MAX_OCT = 22,
	MAX_HEX = 18
};
public:
	conversions():string_int(""){ }
	conversions(const std::string& data):string_int(data){}

	std::string to_base(const size_t& base){
		std::string answer;
		auto reverse = [&](std::string& answer){
			for (int i = 0,j = answer.size()-1; i <=j; ++i,j--){
				char temp = answer[j];
				answer[j] = answer[i];
				answer[i] = temp;
			}
			return answer;
		};
		auto hex_char = [&](char n){
			switch(n){
				case 10 +'0':return 'A';
				case 11 +'0':return 'B';
				case 12 +'0':return 'C';
				case 13 +'0':return 'D';
				case 14 +'0':return 'E';
				case 15 +'0':return 'F';
				default:return n;
			}
		};
		auto hex_int = [&](unsigned char n){
			switch(n){
				case 'A':case 'a':return 10;
				case 'B':case 'b':return 11;
				case 'C':case 'c':return 12;
				case 'D':case 'd':return 13;
				case 'E':case 'e':return 14;
				case 'F':case 'f':return 15;
				default:return n-'0';
			}
		};

		try{

			if(digits[0]=='-') throw "Negetive Not Handled";
			if(!string_int_isvalid()) throw "String Has Unknown Terms";
			if(base != BIN && base != OCT && base != DEC && base != HEX) throw "Output base isnt suppoerted or invalid";

		}catch(const char* message){

			std::cout<<"\n Parse Error:"<<message;
			return "";
		}
		// Main Processing
		// Use Common Conversions with Literals [ string_int class not used As Num is in limit]
		std::string sdigit = get_rem_base_terms();
		base_type string_base = get_base();

		if(string_base == base) return digits;

		if(size()<=MAX_DEC && string_base==DEC || size()<=MAX_BIN && string_base == BIN || size()<=MAX_OCT && string_base == 8 ||size()<=MAX_HEX && string_base == HEX){

			if(string_base == DEC){

				unsigned long long number = 0;
				for(const auto& n:sdigit) number = number * 10 + (n-'0');
				do answer += (base==16 ? hex_char((number % base + '0')) : number % base + '0');
				while(number/=base);

			}else if(string_base == BIN || string_base == OCT || string_base == HEX){

				if(base == DEC){
					
					unsigned long long number=0;
					int c = 0;
					for(const auto& i:sdigit){
					number += hex_int(i) * pow(string_base,sdigit.size() - (++c));}
					while(number){
					answer += number % 10 + '0';
					number /= 10;
					}
				}
			}
		}
		// If Number Limit Exceeded
		else{
			if(string_base == DEC){
				while(allow_processing_via_rules()){
					answer += hex_char(*this % base + '0');
					*this = *this / base;
				}
			}else if(string_base == BIN || string_base == OCT || string_base == HEX){
				string_int answerplus;
				

			}
		}
		if(answer=="") return "0";
		switch(base){
		case 2:answer += "b0";break;
		case 8:answer += "0";break;
		case 16:answer += "x0";break;
		}
		return reverse(answer);
	}
};
void test(){

	assert(conversions(conversions("0b1111").to_base(10)).to_base(2)=="0b1111");
	assert(conversions(conversions("0xFFFFFF").to_base(10)).to_base(16)=="0xFFFFFF");
	assert(conversions(conversions("15").to_base(2)).to_base(10)=="15");
	assert(conversions(conversions("017").to_base(10)).to_base(8)=="017");
	std::cout<<"Tests Passed...\n";

}
int main(){

	test();

	const char *dec_inp = "92345678912345678914877894974987897897498748487498789478748478"; // Big Int

	std::cout<<"\n\n Bignum Is only supported for decimal input mode at present\n\n";
	std::cout<<"\n InputMode\tOutputMode\tInputStr \t\t\t OutputStr\n";
	std::cout<<"\n Decimal\tBinary\t\t"<<dec_inp<<"\t\t"<<conversions(dec_inp).to_base(2);
	std::cout<<"\n Decimal\tOctal\t\t"<<dec_inp<<"\t\t"<<conversions(dec_inp).to_base(8);
	std::cout<<"\n Decimal\tHex\t\t"<<dec_inp<<"\t\t"<<conversions(dec_inp).to_base(16);

	//----------------------------------------------------------------------------
	const char *bin_inp = "0b1111";
	std::cout<<"\n Binary \tDecimal \t"<<bin_inp<<"\t\t\t\t"<<conversions(bin_inp).to_base(10);
	const char* oct_inp = "017";
	std::cout<<"\n Octal  \tDecimal \t"<<oct_inp<<"\t\t\t\t"<<conversions(oct_inp).to_base(10);
	const char* hex_inp = "0x11ff22";
	std::cout<<"\n Hex    \tDecimal \t"<<hex_inp<<"\t\t\t"<<conversions(hex_inp).to_base(10);

	//Error Checkings...
	std::cout<<'\n'<<conversions("0b8784787").to_base(10);
	std::cout<<'\n'<<conversions("0898989").to_base(10);
	std::cout<<'\n'<<conversions("0xFGJK").to_base(22);
	getc(stdin);
}
	

