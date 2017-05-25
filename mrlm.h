#ifndef MR_LM_H
#define  MR_LM_H
#include "metalicensor/generator/license-generator.h"
#include "metalicensor/api/license++.h"
#include "string"

#if _WIN32
#pragma comment(lib,"metalic_generator.lib")
#pragma comment(lib,"metalic_reader.lib")
#endif

class MRLicenseManager
{
public:
	static MRLicenseManager*getInstance()
	{
		MRLicenseManager instance;
		return &instance;
	}
	bool check(const std::string productname = "MR",const std::string licLocation = "MRLic.lic")
	{
		bool bretrun=false;
		LicenseInfo license;
		LicenseLocation licenseLocation;
		licenseLocation.openFileNearModule = false;
		licenseLocation.licenseFileLocation = licLocation.c_str();
		licenseLocation.environmentVariableName = "";
		EVENT_TYPE result = acquire_license(productname.c_str(), licenseLocation, &license);
		if (LICENSE_OK == result)
		{
			bretrun=true;
		}
		return bretrun;
	}
private:
	MRLicenseManager(){}
	~MRLicenseManager(){}
};

#endif