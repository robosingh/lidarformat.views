/***********************************************************************

This file is part of the LidarFormat project source files.

LidarFormat is an open source library for efficiently handling 3D point
clouds with a variable number of attributes at runtime. LidarFormat is
distributed under the CeCILL-B licence. See Licence_CeCILL-B_V1-en.txt
or http://www.cecill.info for more details.


Homepage:

	https://fullanalyze.ign.fr/trac/LidarFormat

Copyright:

	Institut Geographique National & CEMAGREF (2009)

Author:

	Adrien Chauve



This software is governed by the CeCILL-B license under French law and
abiding by the rules of distribution of free software.  You can  use,
modify and/ or redistribute the software under the terms of the CeCILL-B
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info".

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability.

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or
data to be ensured and,  more generally, to use and operate it in the
same conditions as regards security.

The fact that you are presently reading this means that you have had
knowledge of the CeCILL-B license and that you accept its terms.

***********************************************************************/

/*
 * LidarDataView.hpp
 *
 *  Created on: 28 juil. 2009
 *      Author: ndavid
 */

#ifndef LIDARDATAVIEW_HPP_
#define LIDARDATAVIEW_HPP_

template<typename AttType>
class LidarDataAttView{
	public :
	 	typedef AttViewIterator<AttType> iterator;
	 	LidarDataAttView(boost::shared_ptr<LidarDataContainer> data, int offset, int stride) :
	 		m_data_ptr(data),
	 		m_att_offset(offset),
	 		m_att_stride(stride) {}
		iterator begin()
			{
				char * raw_begin_att=m_data_ptr->rawData() + m_att_offset;
				return iterator(raw_begin_att, m_att_stride);
			}
		iterator end()
			{
				int size=m_data_ptr->size();
				char * raw_end_data=m_data_ptr->rawData() + m_att_offset+m_att_stride*size;
				return iterator(raw_end_att, m_att_stride);
			}

	private :
		boost::shared_ptr<LidarDataContainer> m_data_ptr;
		int m_att_offset;
		int m_att_stride;
};


#endif /* LIDARDATAVIEW_HPP_ */
