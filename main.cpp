#include<iostream>
#include"Trans.h"
#include"NFA.h"
#include"Re2NFA.h"
#include"DFA.h"
#include"NFA2DFA.h"
#include"DFAMinimal.h"
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

	return 0;
}