package com.balabas.smarthouse.server.entity.control;

import java.util.List;
//import static com.balabas.smarthouse.server.entity.control.PredicatePreffix.OR;

public interface IPredicateChain extends IAbstractPredicate {

	List<IAbstractPredicate> getPredicates();

	void setPredicates(List<IAbstractPredicate> predicates);

	default boolean test() {
		List<IAbstractPredicate> prs = getPredicates();

		if (prs == null || prs.size() == 0) {
			return false;
		}

		boolean result = prs.get(0).test();

		if (prs.size() > 1) {
			for (int i = 1; i < prs.size(); i++) {
				IAbstractPredicate pr = prs.get(i);

				switch (pr.getPreffix()) {
				case OR:
					result = result || pr.test();
					break;
				default:
					result = result && pr.test();
					break;
				}
			}
		}

		return result;
	}
}
