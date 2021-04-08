#include "seims.h"
#include "DepressionFS.h"
#include "text.h"

DepressionFS::DepressionFS(void) : m_nCells(-1),
                                   m_depCo(NODATA_VALUE), m_depCap(NULL), m_pet(NULL), m_ei(NULL),
                                   m_sd(NULL), m_sr(NULL), m_ed(NULL), m_checkInput(true) {
}

DepressionFS::~DepressionFS(void) {
    Release1DArray(m_sd);
    Release1DArray(m_sr);
    Release1DArray(m_storageCapSurplus);
    Release1DArray(m_ed);
}

bool DepressionFS::CheckInputData(void) {
    if (m_date == -1) {
        throw ModelException(MID_DEP_FS, "CheckInputData", "You have not set the time.");
    }
    if (m_nCells <= 0) {
        throw ModelException(MID_DEP_FS, "CheckInputData", "The cell number of the input can not be less than zero.");
    }
    if (m_depCo == NODATA_VALUE) {
        throw ModelException(MID_DEP_FS, "CheckInputData",
                             "The parameter: initial depression storage coefficient has not been set.");
    }
    if (m_depCap == NULL) {
        throw ModelException(MID_DEP_FS, "CheckInputData",
                             "The parameter: depression storage capacity has not been set.");
    }
#ifndef STORM_MODE
    if (m_pet == NULL) {
        throw ModelException(MID_DEP_FS, "CheckInputData", "The parameter: PET has not been set.");
    }
    if (m_ei == NULL) {
        throw ModelException(MID_DEP_FS, "CheckInputData",
                             "The parameter: evaporation from the interception storage has not been set.");
    }
  	if (m_ed == NULL) {
		throw ModelException(MID_DEP_FS, "CheckInputData",
                             "The parameter: DEET from the interception storage has not been set.");
	}
#endif /* not STORM_MODE */
    return true;
}

void DepressionFS:: initialOutputs() {
    if (m_nCells <= 0) {
        throw ModelException(MID_DEP_FS, "initialOutputs", "The cell number of the input can not be less than zero.");
    }
    if (m_sd == NULL) {
        m_sd = new float[m_nCells];
        m_sr = new float[m_nCells];
        m_storageCapSurplus = new float[m_nCells];
        m_ed = new float[m_nCells];
#pragma omp parallel for
        for (int i = 0; i < m_nCells; ++i) {
            m_sd[i] = m_depCo * m_depCap[i];
            m_sr[i] = 0.0f;
        }
    }
}

int DepressionFS::Execute() {
    initialOutputs();
    if (m_checkInput) {
        CheckInputData();
        m_checkInput = false;
    }

#pragma omp parallel for
    for (int i = 0; i < m_nCells; ++i) {
        // sr is temporarily used to stored the water depth including the depression storage
        float hWater = m_sr[i];
        if (hWater <= m_depCap[i]) {
            m_sd[i] = hWater;
            m_sr[i] = 0.f;
        } else {
            m_sd[i] = m_depCap[i];
            m_sr[i] = hWater - m_depCap[i];
        }
        m_storageCapSurplus[i] = m_depCap[i] - m_sd[i];
        if (m_sd[i] > 0) { //This section is taken from DEP_LINSLEY 
			if (m_pet[i] - m_ei[i] < m_sd[i]) {
				m_ed[i] = m_pet[i] - m_ei[i];
			}
			else {
				m_ed[i] = m_sd[i];
			}
		}
		else {
			m_ed[i] = 0.f;
		}
    }
    return 0;
}

bool DepressionFS::CheckInputSize(const char *key, int n) {
    if (n <= 0) {
        //StatusMsg("Input data for "+string(key) +" is invalid. The size could not be less than zero.");
        return false;
    }
    if (m_nCells != n) {
        if (m_nCells <= 0) { m_nCells = n; }
        else {
            //StatusMsg("Input data for "+string(key) +" is invalid. All the input data should have same size.");
            ostringstream oss;
            oss << "Input data for " + string(key) << " is invalid with size: " << n << ". The origin size is " <<
                m_nCells << ".\n";
            throw ModelException(MID_DEP_FS, "CheckInputSize", oss.str());
        }
    }
    return true;
}

void DepressionFS::SetValue(const char *key, float data) {
    string sk(key);
    if (StringMatch(sk, VAR_DEPREIN)) {
        m_depCo = data;
    } else {
        throw ModelException(MID_DEP_FS, "SetValue", "Parameter " + sk
            + " does not exist in current module. Please contact the module developer.");
    }
}

void DepressionFS::Set1DData(const char *key, int n, float *data) {
    //check the input data
    CheckInputSize(key, n);
    string sk(key);
    if (StringMatch(sk, VAR_DEPRESSION)) {
        m_depCap = data;
    } else if (StringMatch(sk, VAR_PET)) {
        m_pet = data;
    } else if (StringMatch(sk, VAR_INLO)) {
		m_ei = data;
    } else {
        throw ModelException(MID_DEP_FS, "Set1DData", "Parameter " + sk
            + " does not exist in current module. Please contact the module developer.");
    }
}

void DepressionFS::Get1DData(const char *key, int *n, float **data) {
    InitialOutputs();
    string sk(key);
    *n = m_nCells;
    if (StringMatch(sk, VAR_DPST)) {
        *data = m_sd;
    } else if (StringMatch(sk, VAR_SURU)) {
        *data = m_sr;
    } else if (StringMatch(sk, VAR_STCAPSURPLUS)) {
        *data = m_storageCapSurplus;
    } else if (StringMatch(sk, VAR_DEET)) {
		*data = m_ed;
    } else {
        throw ModelException(MID_DEP_FS, "Get1DData", "Output " + sk
            + " does not exist in current module. Please contact the module developer.");
    }
}

