/*!
 * \brief Define MetadataInfo class used by modules
 * \author Junzhi Liu, Liangjun Zhu
 * \version 1.1
 * \date June 2010
 * \revision 2018-3-16 lj Simplify code, and add In/Output parameters related for MPI version.
 */
#ifndef SEIMS_METADATA_INFO_H
#define SEIMS_METADATA_INFO_H

#include "utilities.h"
#include "MetadataInfoConst.h"

using namespace std;

/*!
 * \ingroup util
 * \struct ModelClass
 * \brief Module basic description
 */
struct ModelClass {
public:
    ModelClass(): Name(""), Description("") {};
public:
    string Name;
    string Description;
};

/*!
 * \ingroup util
 * \struct Information
 *
 * \brief Module development information class
 */
struct Information {
public:
    Information() : Id(""), Name(""), Description(""), Version(""), Author(""),
                    EMail(""), Website(""), Helpfile("") {};
public:
    string Id;             ///< Module ID
    string Name;           ///< Module Name
    string Description;    ///< Module Description
    string Version;        ///< Module Version
    string Author;         ///< Author
    string EMail;          ///< Email
    string Website;        ///< Website
    string Helpfile;       ///< Helpfile
};

/*!
 * \ingroup util
 * \struct baseParameter
 *
 * \brief Basic model parameter information
 */
struct baseParameter {
public:
    baseParameter() : Name(""), Units(""), Description(""), Dimension(DT_Unknown) {};
public:
    string Name;                 ///< Name
    string Units;                ///< Units
    string Description;          ///< Description
    dimensionTypes Dimension;    ///< Data dimension type
};

/*!
 * \ingroup util
 * \struct Parameter
 *
 * \brief Model parameter information class
 */
struct Parameter: public baseParameter {
public:
    Parameter() : baseParameter(), Source("") {};
public:
    string Source;               ///< Source type
};

/*!
 * \ingroup util
 * \struct InputVariable
 *
 * \brief Input variable information class
 */
struct InputVariable: public Parameter {
public:
    InputVariable() : Parameter(), tfType(TF_Whole) {};
public:
    transferTypes tfType;
};

/*!
 * \ingroup Util
 * \struct OutputVariable
 * \brief Output variable information class
 */
struct OutputVariable: public baseParameter {
public:
    OutputVariable() : baseParameter(), tfType(TF_Whole) {};
public:
    transferTypes tfType;
};

/*!
 * \ingroup Util
 * \struct InOutputVariable
 * \brief Input and output variable information class
 */
struct InOutputVariable: public InputVariable {
public:
    InOutputVariable() : InputVariable() {};
};

/*!
 * \ingroup Util
 * \class MetadataInfo
 * \brief Metadata information of module
 */
class MetadataInfo {
private:
    string m_strSchemaVersion;                ///< latest XML schema version supported by this class
    ModelClass m_oClass;                      ///< class name for the module
    Information m_Info;                       ///< the general information for the module
    vector<Parameter> m_vParameters;         ///< list of parameters for the module
    vector<InputVariable> m_vInputs;         ///< list of input parameters for the module
    vector<OutputVariable> m_vOutputs;       ///< list of output parameters for the module
    vector<InOutputVariable> m_vInOutputs;   ///< list of In/Output parameters for the module for MPI version
    vector<ModelClass> m_vDependencies;      ///< list of dependency classes for the module

    void OpenTag(string name, string attributes, int indent, string *sb);

    void CloseTag(string name, int indent, string *sb);

    void FullTag(const string &name, int indent, string &content, string *sb);

    void WriteClass(int indent, string *sb);

    void WriteInformation(int indent, string *sb);

    void WriteInputs(int indent, string *sb);

    void WriteOutputs(int indent, string *sb);

    void WriteInOutputs(int indent, string *sb);

    void WriteParameters(int indent, string *sb);

    void WriteDependencies(int indent, string *sb);

    void WriteXMLHeader(string *sb);

    void DimensionTag(string tag, int indent, dimensionTypes dimType, string *sb);

    void TransferTypeTag(string tag, int indent, transferTypes tfType, string *sb);

public:
    MetadataInfo() { m_strSchemaVersion = "0.4"; };

    ~MetadataInfo();

    string SchemaVersion() { return m_strSchemaVersion; };

    void SetClass(const char* name, const char* description);

    string GetClassName() { return m_oClass.Name; };

    string GetClassDescription() { return m_oClass.Description; };

    void SetID(const char* ID) { m_Info.Id = ID; };

    string GetID() { return m_Info.Id; };

    void SetName(const char* name) { m_Info.Name = name; };

    string GetName() { return m_Info.Name; };

    void SetDescription(const char* description) { m_Info.Description = description; };

    string GetDescription() { return m_Info.Description; };

    void SetVersion(const char* version) { m_Info.Version = version; };

    string GetVersion() { return m_Info.Version; };

    void SetAuthor(const char* author) { m_Info.Author = author; };

    string GetAuthor() { return m_Info.Author; };

    void SetEmail(const char* email) { m_Info.EMail = email; };

    string GetEmail() { return m_Info.EMail; };

    void SetWebsite(const char* site) { m_Info.Website = site; };

    string GetWebsite() { return m_Info.Website; };

    void SetHelpfile(const char* file) { m_Info.Helpfile = file; };

    string GetHelpfile() { return m_Info.Helpfile; };

    /************ INPUT PARAMETERS FROM OTHER MODULES ************/

    int GetInputCount() { return (int) m_vInputs.size(); };

    int AddInput(const char* name, const char* units, const char* desc, const char* source, dimensionTypes dimType, transferTypes tfType = TF_Whole);

    string GetInputName(int index) { return (index >= 0 && index < m_vInputs.size())? m_vInputs[index].Name : ""; };

    string GetInputUnits(int index) { return (index >= 0 && index < m_vInputs.size())? m_vInputs[index].Units : ""; };

    string GetInputDescription(int index) { return (index >= 0 && index < m_vInputs.size())? m_vInputs[index].Description : ""; };

    string GetInputSource(int index) { return (index >= 0 && index < m_vInputs.size())? m_vInputs[index].Source : ""; };

    dimensionTypes GetInputDimension(int index) { return (index >= 0 && index < m_vInputs.size())? m_vInputs[index].Dimension : DT_Unknown; };

    transferTypes GetInputTfType(int index) { return (index >= 0 && index < m_vInputs.size())? m_vInputs[index].tfType : TF_Whole; };

    InputVariable GetInput(int index) { return (index >= 0 && index < m_vInputs.size())? m_vInputs[index]: InputVariable(); };;

    /************ OUTPUT PARAMETERS ************/

    int GetOutputCount() { return (int) m_vOutputs.size(); };

    int AddOutput(const char* name, const char* units, const char* desc, dimensionTypes dimType, transferTypes tfType = TF_Whole);

    string GetOutputName(int index) { return (index >= 0 && index < m_vOutputs.size())? m_vOutputs[index].Name : ""; };

    string GetOutputUnits(int index) { return (index >= 0 && index < m_vOutputs.size())? m_vOutputs[index].Units : ""; };

    string GetOutputDescription(int index) { return (index >= 0 && index < m_vOutputs.size())? m_vOutputs[index].Description : ""; };

    dimensionTypes GetOutputDimension(int index) { return (index >= 0 && index < m_vOutputs.size())? m_vOutputs[index].Dimension : DT_Unknown; };

    transferTypes GetOutputTfType(int index) { return (index >= 0 && index < m_vOutputs.size())? m_vOutputs[index].tfType : TF_Whole; };

    OutputVariable GetOutput(int index) { return (index >= 0 && index < m_vOutputs.size())? m_vOutputs[index] : OutputVariable(); };

    /************ IN/OUTPUT PARAMETERS ************/

    int GetInOutputCount() { return (int) m_vInOutputs.size(); };

    int AddInOutput(const char* name, const char* units, const char* desc, dimensionTypes dimType, transferTypes tfType = TF_Whole);
    
    string GetInOutputName(int index) { return (index >= 0 && index < m_vInOutputs.size())? m_vInOutputs[index].Name : ""; };

    string GetInOutputUnits(int index) { return (index >= 0 && index < m_vInOutputs.size())? m_vInOutputs[index].Units : ""; };

    string GetInOutputDescription(int index) { return (index >= 0 && index < m_vInOutputs.size())? m_vInOutputs[index].Description : ""; };

    dimensionTypes GetInOutputDimension(int index) { return (index >= 0 && index < m_vInOutputs.size())? m_vInOutputs[index].Dimension : DT_Unknown; };

    transferTypes GetInOutputTfType(int index) { return (index >= 0 && index < m_vInOutputs.size())? m_vInOutputs[index].tfType : TF_Whole; };
    
    InOutputVariable GetInOutput(int index) { return (index >= 0 && index < m_vInOutputs.size())? m_vInOutputs[index] : InOutputVariable(); };

    /************ PARAMETERS FROM DATABASE ************/

    int GetParameterCount() { return (int) m_vParameters.size(); };

    int AddParameter(const char* name, const char* units, const char* desc, const char* source, dimensionTypes dimType);

    string GetParameterName(int index) { return (index >= 0 && index < m_vParameters.size())? m_vParameters[index].Name : ""; };

    string GetParameterUnits(int index) { return (index >= 0 && index < m_vParameters.size())? m_vParameters[index].Units : ""; };

    string GetParameterDescription(int index) { return (index >= 0 && index < m_vParameters.size())? m_vParameters[index].Description : ""; };

    string GetParameterSource(int index) { return (index >= 0 && index < m_vParameters.size())? m_vParameters[index].Source : ""; };

    dimensionTypes GetParameterDimension(int index) { return (index >= 0 && index < m_vParameters.size())? m_vParameters[index].Dimension : DT_Unknown; };

    Parameter GetParameter(int index) { return (index >= 0 && index < m_vParameters.size())? m_vParameters[index] : Parameter(); };

    /************ DEPENDENT MODULES ************/

    int GetDependencyCount() { return (int) m_vDependencies.size(); };

    int AddDependency(const char* name, const char* description);

    string GetDependencyName(int index) { return (index >= 0 && index < m_vDependencies.size())? m_vDependencies[index].Name : ""; };

    string GetDependencyDescription(int index) { return (index >= 0 && index < m_vDependencies.size())? m_vDependencies[index].Description : ""; };

    ModelClass GetDependency(int index) { return (index >= 0 && index < m_vDependencies.size())? m_vDependencies[index] : ModelClass(); };

    string GetXMLDocument();
};

#endif /* SEIMS_METADATA_INFO_H */
