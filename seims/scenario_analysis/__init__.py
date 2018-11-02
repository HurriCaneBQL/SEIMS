#! /usr/bin/env python
# -*- coding: utf-8 -*-
"""
/***************************************************************************
 pySEIMS
                             Python APIs for SEIMS
 Scenario Analysis by NSGA-II algorithm of DEAP, parallelized by SCOOP
                              -------------------
        author               : Liangjun Zhu, Huiran Gao
        copyright            : (C) 2018 by Lreis, IGSNRR, CAS
        email                : zlj@lreis.ac.cn
 ******************************************************************************
 *                                                                            *
 *   SEIMS is distributed for Research and/or Education only, any commercial  *
 *   purpose will be FORBIDDEN. SEIMS is an open-source project, but without  *
 *   ANY WARRANTY, WITHOUT even the implied warranty of MERCHANTABILITY or    *
 *   FITNESS for A PARTICULAR PURPOSE.                                        *
 *   See the GNU General Public License for more details.                     *
 *                                                                            *
 ******************************************************************************/
"""

__author__ = "Liangjun Zhu, Huiran Gao"
__description__ = "Scenario Analysis by NSGA-II algorithm"
__version__ = "1.2"
__revision__ = "1.2.0"

_DEBUG = True  # type: bool # Print information for debugging

BMPS_RULE_METHODS = ['RDM', 'SUIT', 'UPDOWN', 'SLPPOS']
"""The available rule methods for BMPs configuration.
- RDM: Config all available BMPs on each spatial unit randomly.
- SUIT: Config the suitable BMPs on each spatial unit randomly, which is the default
 knowledge-based rule method.
- UPDOWN: Config the suitable BMPs according to upstream-downstream relationships,
i.e., if a unit has been configured with one BMP, its adjacent upstream units should not be
 configured with BMP, otherwise will be randomly selected and configured according to their
 landuse type (Wu et al., 2018, WRM).
- SLPPOS: Config the suitable BMPs according to the slope position sequences along hillslope,
i.e., the effective grade of the BMP configured on the downslope position should be greater or
 equal to that of the BMP configured on its adjacent upslope position (Qin et al., 2018, JSWC).
"""
