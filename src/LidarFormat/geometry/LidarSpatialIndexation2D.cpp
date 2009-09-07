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
	
Contributors:

	Nicolas David, Olivier Tournaire



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

/*!
 * \file LidarSpatialIndexation2D.cpp
 * \brief
 * \author Adrien Chauve
 * \date 26 janv. 2009
 */

#include <boost/bind.hpp>

#include "LidarFormat/LidarDataContainer.h"

#include "LidarSpatialIndexation2D.h"

namespace Lidar
{

///Voisinage grossier carré XY autour du centre, de demi-côté approxNeighborhoodSize, puis raffiné avec la fonction passée en paramètre dans le sous-ensemble grossier
void LidarSpatialIndexation2D::GetCenteredNeighborhood(NeighborhoodListeType &list, const TPoint2D<float> &centre, const float approxNeighborhoodSize, const NeighborhoodFunctionType isInside ) const
{
	list.clear();

	//Récupération des pixels à visiter
	int colonne, ligne;
	m_ori.MapToImage( centre.x, centre.y, colonne, ligne );

	const int tailleVoisinage = static_cast<int> ( std::ceil( approxNeighborhoodSize / m_resolution ) );

	const int colMin = std::max( 0, colonne - tailleVoisinage );
	const int colMax = std::min( m_griddedData.GetTaille().x - 1, colonne + tailleVoisinage );
	const int ligMin = std::max( 0, ligne - tailleVoisinage );
	const int ligMax = std::min( m_griddedData.GetTaille().y - 1, ligne + tailleVoisinage );

	const unsigned int evalNbPoints = (unsigned int)( (colMax-colMin+1)*(ligMax-ligMin+1)*m_resolution*m_nbPointsParM2 );
	list.reserve(evalNbPoints);

	const LidarConstIteratorXYZ<float> beginXYZ = m_lidarContainer.beginXYZ<float>();

	for (int col = colMin; col <= colMax; ++col)
	{
		for (int lig = ligMin; lig <= ligMax; ++lig)
		{
			std::vector<unsigned int>::const_iterator itb = m_griddedData(col, lig).begin();
			const std::vector<unsigned int>::const_iterator ite = m_griddedData(col, lig).end();
			for (; itb != ite; ++itb)
			{
				const LidarConstIteratorXYZ<float> itXYZ(beginXYZ + *itb);
				if (isInside(itXYZ.x(), itXYZ.y(), itXYZ.z()))
					list.push_back(*itb);
			}

		}
	}
}

void LidarSpatialIndexation2D::findBBox()
{

	m_bboxMin.x = std::numeric_limits<float>::max();
	m_bboxMin.y = std::numeric_limits<float>::max();
	m_bboxMax.x = -std::numeric_limits<float>::max();
	m_bboxMax.y = -std::numeric_limits<float>::max();


	LidarConstIteratorXYZ<float> itb = m_lidarContainer.beginXYZ<float>();
	const LidarConstIteratorXYZ<float> ite = m_lidarContainer.endXYZ<float>();


	for (; itb != ite; ++itb)
	{
		m_bboxMin.x = std::min(m_bboxMin.x, itb.x());
		m_bboxMin.y = std::min(m_bboxMin.y, itb.y());
		m_bboxMax.x = std::max(m_bboxMax.x, itb.x());
		m_bboxMax.y = std::max(m_bboxMax.y, itb.y());
	}

	std::cout << "LidarSpatialIndexation2D  bbox = " << m_bboxMin << " ; " << m_bboxMax << std::endl;

}


void LidarSpatialIndexation2D::fillData()
{
	int col, ligne;

	LidarConstIteratorXYZ<float> itb = m_lidarContainer.beginXYZ<float>();
	const LidarConstIteratorXYZ<float> ite = m_lidarContainer.endXYZ<float>();

	const LidarConstIteratorXYZ<float> itbegin = m_lidarContainer.beginXYZ<float>();

	for (; itb != ite; ++itb)
	{
		m_ori.MapToImage( itb.x(), itb.y(), col, ligne );

//		std::cout << *itx << "\t" << *ity << "\t" << col << "\t" << ligne << std::endl;
		//dans le cas où la bbox n'a pas été calculée mais fournie dan le constructeur, il faut tester si on sort de la grille
		if(col>=0 && ligne>=0 && col<m_griddedData.GetTaille().x && ligne<m_griddedData.GetTaille().y)
			m_griddedData(col, ligne).push_back(itb - itbegin);
	}

}

LidarSpatialIndexation2D::LidarSpatialIndexation2D(const LidarDataContainer& lidarContainer):
	RasterSpatialIndexation(),
	m_lidarContainer(lidarContainer)
{

}


LidarSpatialIndexation2D::~LidarSpatialIndexation2D()
{

}


} //namespace Lidar
