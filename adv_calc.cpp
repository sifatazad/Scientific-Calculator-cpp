/*
  Prepared by Sifat Ibn Azad
  ME,BUET
  24 October 2026
*/

#include<bits/stdc++.h>
using namespace std;

static stack<string> st;
static queue<string> qu;

static void remove_space(string &s) {
    s.erase(ranges::remove(s,' ').begin(),s.end());
}

static void make_lower_case(string &s) {
    int n= static_cast<int>(s.length());
    for (int i=0; i<n; i++) {
        if (s[i]>=65 && s[i]<=90) {
            s[i]+=32;
        }
    }
}

static bool is_valid(const string &s) {
    int n=static_cast<int>(s.length());
    int first=0, second=0, third=0;

    for (int i=0; i<n; i++) {
        if (s[i]=='(') {
            first++;
        }
        else if (s[i]=='{') {
            second++;
        }
        else if (s[i]=='[') {
            third++;
        }
        else if (s[i]==')') {
            first--;
        }
        else if (s[i]=='}') {
            second--;
        }
        else if (s[i]==']') {
            third--;
        }
        if (first<0 || second<0 || third<0) {
            ostringstream oss;
            oss<<"Syntax Error!\nBracket mismatch at index: "<<(i+1)<<endl;;
            throw runtime_error(oss.str());
        }
    }
    if (first>0 || second>0 || third>0) {
        ostringstream oss;
        oss<<"Syntax Error!\nExtra "<<first+second+third<<" opening Bracket used."<<endl;
        throw runtime_error(oss.str());
    }

    int mod=0;
    for (int i=0; i<n; i++) {
        if (s[i]=='|')
            mod++;
    }
    if (mod%2) {
        ostringstream oss;
        oss<<"Syntax Error!\nExtra Modulus (|) used."<<endl;
        throw runtime_error(oss.str());
    }
    return true;
}

static bool is_problem(const char &c) {
    if (c=='+' || c=='-' || c=='*' || c=='/' || c=='%' ||
        c=='^'|| c=='(' || c=='{' || c=='[')
        return true;
    return false;
}

static bool is_digit(const char ch) {
    if (ch>=48 && ch<=57)
        return true;
    return false;
}

static bool is_number(const string &s) {
    if (s[0]=='.' || is_digit(s[0]))
        return true;
    return false;
}

static bool is_opening_bracket(const string &s) {
    return (s=="(" || s=="{" || s=="[");
}

static bool is_closing_bracket(const string &s) {
    return (s==")" || s=="}" || s=="]");
}

static int get_level(const string &s) {
    if (s=="(" || s==")" || s=="{" || s=="}" || s=="[" || s=="]") {
        return 1;
    }
    if(s=="log" || s=="sin" || s=="cos" || s=="tan" || s=="sqrt" || s=="abs") {
        return 2;
    }
    if (s=="u-" || s=="^") {
        return 3;
    }
    if (s=="*" || s=="/" || s=="%") {
        return 4;
    }
    if (s=="+" || s=="-") {
        return 5;
    }
    return 0;
}

static void pre_process(string &s) {
    for (int i=0; i<static_cast<int>(s.length()); i++) {
        if (i!=0 && is_opening_bracket(string(1,s[i]))) {
            if (is_digit(s[i-1])) {
                s.insert(i,"*");
                i++;
            }
        }
    }

    for (int i=0; i<static_cast<int>(s.length()); i++) {
        string s3=s.substr(i,3);
        if (i!=0 && (s3=="sin" || s3=="cos" || s3=="tan" || s3=="log")) {
            if (is_digit(s[i-1])) {
                s.insert(i,"*");
                i+=4;
            }
        }
    }

    for (int i=0; i<static_cast<int>(s.length()); i++) {
        string s4=s.substr(i,4);
        if (i!=0 && s4=="sqrt") {
            if (is_digit(s[i-1])) {
                s.insert(i,"*");
                i+=5;
            }
        }
    }

    string ans;
    int n= static_cast<int>(s.length());
    for (int i=0; i<n; i++) {
        if (s[i]=='|') {
            bool is_opening=false;
            if (ans.empty()) {
                is_opening=true;
            }
            else {
                char last=ans.back();
                if (is_problem(last)) {
                    is_opening=true;
                }
            }
            if (is_opening) {
                ans.push_back('a');
                ans.push_back('b');
                ans.push_back('s');
                ans.push_back('(');
            }
            else {
                ans.push_back(')');
            }
        }
        else {
            ans.push_back(s[i]);
        }
    }

    s=ans;
    // cout<<s<<endl;
}

static void postfix(const string &s) {
    int n=static_cast<int> (s.length());
    for (int i=0; i<n; i++) {
        string s3=s.substr(i,3);
        string s4=s.substr(i,4);

        if ((i==0 && s[i]=='-') || (i>0 && s[i]=='-' && is_problem(s[i-1]))) {
            while (true) {
                if (st.empty() || is_opening_bracket(st.top())) {
                    break;
                }
                int curr_level=get_level("u-");
                int top_level=get_level(st.top());
                if (curr_level>top_level) {
                    qu.emplace(st.top());
                    st.pop();
                }
                else {
                    break;
                }
            }
            st.emplace("u-");
        }
        else if (is_digit(s[i]) || s[i]=='.') {
            int idx=n;
            for (int j=i+1; j<n; j++) {
                if (s[j]!='.' && !is_digit(s[j])) {
                    idx=j;
                    break;
                }
            }
            qu.emplace(s.substr(i,idx-i));
            i=idx-1;
        }
        else if (i<n-2 && s[i]=='p' && s[i+1]=='i') {
            qu.emplace(to_string(M_PI));
            i++;
        }
        else if (s[i]=='e') {
            qu.emplace(to_string(M_E));
        }
        else if (is_closing_bracket(string(1,s[i]))) {
            if (s[i]==')') {
                while (true) {
                    if (st.top()=="(") {
                        st.pop();
                        break;
                    }
                    qu.emplace(st.top());
                    st.pop();
                }
            }
            else if (s[i]=='}') {
                while (true) {
                    if (st.top()=="{") {
                        st.pop();
                        break;
                    }
                    qu.emplace(st.top());
                    st.pop();
                }
            }
            else if (s[i]==']') {
                while (true) {
                    if (st.top()=="[") {
                        st.pop();
                        break;
                    }
                    qu.emplace(st.top());
                    st.pop();
                }
            }
        }
        else if (s3=="sin" || s3=="cos" || s3=="tan" || s3=="log" || s3=="abs") {
            st.emplace(s3);
            i+=2;
        }
        else if (s4=="sqrt") {
            st.emplace(s4);
            i+=3;
        }
        else if (s[i]=='^') {
            while (true) {
                if (st.empty() || is_opening_bracket(st.top())) {
                    break;
                }
                int curr_level=get_level("^");
                int top_level=get_level(st.top());
                if (curr_level>top_level) {
                    qu.emplace(st.top());
                    st.pop();
                }
                else
                    break;
            }
            st.emplace("^");
        }
        else {
            while (true) {
                if (st.empty() || is_opening_bracket(st.top())) {
                    break;
                }
                int curr_level=get_level(string(1,s[i]));
                int top_level=get_level(st.top());
                if (curr_level>=top_level) {
                    qu.emplace(st.top());
                    st.pop();
                }
                else
                    break;
            }
            st.emplace(1,s[i]);
        }
    }
    while (true) {
        if (st.empty()) {
            break;
        }
        qu.emplace(st.top());
        st.pop();
    }
}

static double calculate(string &s) {
    make_lower_case(s);
    remove_space(s);
    if(is_valid(s)) {
        pre_process(s);
        postfix(s);
        while (true) {
            if (qu.empty()) {
                break;
            }
            if (is_number(qu.front())) {
                st.emplace(qu.front());
                qu.pop();
            }
            else {
                const string &op=qu.front();
                qu.pop();
                if (op=="sin") {
                    double v=sin(stod(st.top()));
                    st.pop();
                    st.emplace(to_string(v));
                }
                else if (op=="cos") {
                    double v=cos(stod(st.top()));
                    st.pop();
                    st.emplace(to_string(v));
                }
                else if (op=="tan") {
                    double cnfm=stod(st.top());
                    if (abs(cnfm-M_PI/2)<10e-9) {
                        throw runtime_error("Math Error!\n"
                               "Input angle of tan() is very close to pi/2\n");
                    }
                    double v=tan(cnfm);
                    st.pop();
                    st.emplace(to_string(v));
                }
                else if (op=="log") {
                    double cnfm=stod(st.top());
                    if (abs(cnfm)<10e-9) {
                        throw runtime_error("Math Error!\n"
                        "The number of natural logarithm is very close to 0.\n");
                    }
                    else if (cnfm<0) {
                        throw runtime_error("Math Error!\n"
                        "The number of natural logarithm is less than 0.\n");
                    }
                    double v=log(cnfm);
                    st.pop();
                    st.emplace(to_string(v));
                }
                else if (op=="sqrt") {
                    double cnfm=stod(st.top());
                    if (cnfm<0) {
                        throw runtime_error("Math Error!\n"
                        "The number inside square root is less than 0.\n");
                    }
                    double v=sqrt(cnfm);
                    st.pop();
                    st.emplace(to_string(v));
                }
                else if (op=="abs") {
                    double v=abs(stod(st.top()));
                    st.pop();
                    st.emplace(to_string(v));
                }
                else if (op=="u-") {
                    double v=-stod(st.top());
                    st.pop();
                    st.emplace(to_string(v));
                }
                else {
                    double b= stod(st.top());
                    st.pop();
                    double a= stod(st.top());
                    st.pop();
                    if (op=="+") {
                        st.emplace(to_string(a+b));
                    }
                    else if (op=="-") {
                        st.emplace(to_string(a-b));
                    }
                    else if (op=="*") {
                        st.emplace(to_string(a*b));
                    }
                    else if (op=="/") {
                        if (abs(b)<10e-9) {
                            throw runtime_error("Math Error!\n"
                            "The denominator is very close or equals to 0.\n");
                        }
                        st.emplace(to_string(a/b));
                    }
                    else if (op=="%") {
                        if (abs(b)<10e-9) {
                            throw runtime_error("Math Error!\n"
                            "The denominator is very close or equals to 0.\n");
                        }
                        st.emplace(to_string(fmod(a,b)));
                    }
                    else if (op=="^") {
                        if (abs(a)<10e-9 && abs(b)<10e-9) {
                            throw runtime_error("Math Error!\n"
                            "Indeterminate form (0^0).\n");
                        }
                        if (abs(a)<10e-9 && b<0) {
                            throw runtime_error("Math Error!\n"
                            "Indeterminate form (Division by zero) (1/0^n).\n");
                        }
                        st.emplace(to_string(pow(a,b)));
                    }
                }
            }
        }
    }

    double val= stod(st.top());
    st.pop();
    return val;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s="log(-10)";
    try {
        double ans=calculate(s);
        printf("%.4lf\n",ans);
    }
    catch (const exception &e) {
        cout<<e.what()<<endl;
    }
}