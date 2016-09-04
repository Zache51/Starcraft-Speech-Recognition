#pragma once
//#pragma warning(disable: 4996)

#include <sphelper.h>
#include <sapi.h>

#include <string>
#include <vector>
#include <sstream>

class SpeechRecognition
{
private:

	const ULONGLONG grammarId = 0;

	const wchar_t* ruleProduce = L"ruleProduce";
	const wchar_t* ruleListening = L"ruleListening";

	bool m_bInSound;
	bool m_bGotReco;
	bool silentMode;

	HRESULT hr;
	CComPtr<ISpRecognizer> cpRecoEngine;
	ISpRecoContext* recoContext;
	ISpRecoGrammar* recoGrammar;
	CSpEvent srEvent;

	// Used for checking if there are errors
	void checkResult(const HRESULT& result);

	// Separates one big string into smaller strings representing tokens/words
	std::vector<std::string> splitString(std::string str);

	// Initializes the grammar with rules and words
	void initializeGrammar();

public:
	SpeechRecognition();
	virtual ~SpeechRecognition();

	/* 
	Fills vector with words if something is recognized

	*/
	void getCommand(std::vector<int>& voiceCommand);
};

