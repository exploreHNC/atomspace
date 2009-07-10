/*
 * Copyright (C) 2002-2007 Novamente LLC
 * Copyright (C) 2008 by Singularity Institute for Artificial Intelligence
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the exceptions
 * at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef INVERSIONRULE_H
#define INVERSIONRULE_H

namespace opencog { namespace pln {

//template<Type InclusionLink>
class InversionRule : public GenericRule<InversionFormula>
{
protected:
// mutable std::vector<Type> ti;
    Type InclusionLink;

    virtual TruthValue** formatTVarray(const std::vector<Vertex>& premiseArray,
                                       int* newN) const {
        TruthValue** tvs = (TruthValue**)new SimpleTruthValue*[3];

        assert(premiseArray.size() == 1);
        AtomSpaceWrapper *nm = GET_ASW;
        pHandleSeq nodes = nm->getOutgoing(boost::get<pHandle>(premiseArray[0]));

        tvs[0] = (TruthValue*) & (nm->getTV(boost::get<pHandle>(premiseArray[0])));
        tvs[1] = (TruthValue*) & (nm->getTV(nodes[0]));
        tvs[2] = (TruthValue*) & (nm->getTV(nodes[1]));

        return tvs;
    }
    std::vector<BoundVertex> r;

    Rule::setOfMPs o2iMetaExtra(meta outh, bool& overrideInputFilter) const {
        if (!GET_ASW->inheritsType((Type)_v2h(*outh->begin()), InclusionLink))
            return Rule::setOfMPs();

        Rule::MPs ret;
        ret.push_back(atomWithNewType(*outh, InclusionLink));
        tree<Vertex>::sibling_iterator top = ret[0]->begin();
        tree<Vertex>::sibling_iterator right = ret[0]->begin(top);
        tree<Vertex>::sibling_iterator left = right++;
        outh->swap(left, right);

        overrideInputFilter = true;

        return makeSingletonSet(ret);
    }

public:
    //InversionRule(iAtomSpaceWrapper *_destTable)
            //: GenericRule<InversionFormula> (_destTable, false, "InversionRule") {
    InversionRule(iAtomSpaceWrapper *_destTable, Type linkType)
            : GenericRule<InversionFormula> (_destTable, false, "InversionRule"), InclusionLink(linkType) {
        inputFilter.push_back(meta(
                                  new tree<Vertex>(
                                      mva((pHandle)InclusionLink,
                                          mva((pHandle)ATOM),
                                          mva((pHandle)ATOM))
                                  )));
    }
    bool validate2    (MPs& args) const {
        return true;
    }

    virtual meta i2oType(const std::vector<Vertex>& h) const {
        assert(1 == h.size());
        pHandle h0 = boost::get<pHandle>(h[0]);
        /*cprintf(1,"INV OLD ATOM:\n");
        printTree(boost::get<Handle>(h[0]),0,1);
        cprintf(1,"INV New order:\n");
        printTree(child(boost::get<Handle>(h[0]),1),0,1);
        printTree(child(boost::get<Handle>(h[0]),0),0,1);*/
        return meta(new tree<Vertex>(mva((pHandle)GET_ASW->getType(h0),
                                         mva(GET_ASW->getOutgoing(h0, 1)),
                                         mva(GET_ASW->getOutgoing(h0, 0))
                                        )));
    }
    NO_DIRECT_PRODUCTION;
};

}} // namespace opencog { namespace pln {
#endif // INVERSIONRULE_H
