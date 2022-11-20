#include<iostream>
#include"Trans.h"
#include"NFA.h"
#include"Re2NFA.h"
#include"DFA.h"
#include"NFA2DFA.h"
#include"DFAMinimal.h"
#include"NFA2RE.h"
#include"DFA2RE.h"
int main() {
	NFA a, b;
	
	a.set_vertex_cnt(2);
	a.add_transition(0, 1, 'a');
	a.add_final_state(1);
	a.display();
	
	b.set_vertex_cnt(2);
	b.add_transition(0, 1, 'b');
	b.add_final_state(1);
	b.display();

	printf("ab\n");
	NFA ab = concat(a, b);
	ab.display();

	printf("a|b\n");
	NFA a_or_b = or_selection(a, b);
	a_or_b.display();

	printf("a*\n");
	NFA a_kleene = kleene(a);
	a_kleene.display();

	NFA nfa = re2nfa("(a|b)*.b.b.b.(a|b)*");
	nfa.display();

	DFA dfa = nfa2dfa(nfa);
	dfa.display();

	DFA dfa_r = remove_unreachable_state(dfa, { 'a','b' });
	dfa_r.display();

	DFA minimal_dfa = hopcroft(dfa, { 'a','b' });
	minimal_dfa.display();

	//NFA nfa2(
	//	4,
	//	{
	//		{0,1,'a'},{1,2,'b'},{2,1,'c'},{2,2,'d'},{2,3,'e'},{3,2,'f'},{1,3,'g'},{3,1,'h'}
	//	},
	//	{ 3 }
	//);
	//NFA nfa2 = re2nfa("(a|b)*");
	//nfa2.display();
	//std::cout << nfa2re(nfa2) << std::endl;
	DFA dfa2 = hopcroft(nfa2dfa(re2nfa(nfa2re(nfa))), { 'a','b' });
	dfa2.display();

	DFA dfa3 = hopcroft(nfa2dfa(re2nfa(dfa2re(dfa))), { 'a','b' });
	dfa3.display();

	return 0;
}