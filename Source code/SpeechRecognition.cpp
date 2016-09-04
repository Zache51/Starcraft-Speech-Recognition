#include "SpeechRecognition.h"

// Used for checking if there are errors
void SpeechRecognition::checkResult(const HRESULT& result)
{
	if (result == S_OK || result == S_FALSE) {
		return;
	}

	std::string message;

	switch (result) {

	case E_INVALIDARG:
		message = "One or more arguments are invalids.";
		break;

	case E_ACCESSDENIED:
		message = "Acces Denied.";
		break;

	case E_NOINTERFACE:
		message = "Interface does not exist.";
		break;

	case E_NOTIMPL:
		message = "Not implemented method.";
		break;

	case E_OUTOFMEMORY:
		message = "Out of memory.";
		break;

	case E_POINTER:
		message = "Invalid pointer.";
		break;

	case E_UNEXPECTED:
		message = "Unexpecter error.";
		break;

	case E_FAIL:
		message = "Failure";
		break;

	default:
		message = "Unknown : " + std::to_string(result);
	}

	throw std::exception(message.c_str());
}

// Separates one big string into smaller strings representing tokens/words
std::vector<std::string> SpeechRecognition::splitString(std::string str)
{
	std::string temp;
	std::stringstream ss(str);
	std::vector<std::string> words;
	while (ss >> temp) words.push_back(temp);
	return words;
}

void SpeechRecognition::initializeGrammar()
{
	hr = recoContext->CreateGrammar(grammarId, &recoGrammar);
	checkResult(hr);

	WORD langId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
	hr = recoGrammar->ResetGrammar(langId);
	checkResult(hr);
	// TODO: Catch error and use default langId => GetUserDefaultUILanguage()~t

	// Load rules and words from file
	recoGrammar->LoadCmdFromFile(L"terran_grammar.xml", SPLOADOPTIONS::SPLO_STATIC);
	checkResult(hr);

	hr = recoGrammar->SetGrammarState(SPGS_DISABLED);

	// Activate Grammar
	hr = recoGrammar->SetRuleState(ruleListening, 0, SPRS_ACTIVE);
	checkResult(hr);
	hr = recoGrammar->SetRuleState(ruleProduce, 0, SPRS_ACTIVE);
	checkResult(hr);

	hr = recoGrammar->SetGrammarState(SPGS_ENABLED);
}

SpeechRecognition::SpeechRecognition()
{
	//// Initialize COM library
	if (SUCCEEDED(::CoInitialize(nullptr))) 
	{
		// CLSID_SpSharedRecognizer activates all of the windows commands and is not wanted
		// CLSID_SpInprocRecognizer limits the reqognizer to this application
		hr = cpRecoEngine.CoCreateInstance(CLSID_SpInprocRecognizer);


		// Setup audio. This is necessary when using CLSID_SpInprocRecognizer

		// create default audio object
		CComPtr<ISpAudio> cpAudio;
		SpCreateDefaultObjectFromCategoryId(SPCAT_AUDIOIN, &cpAudio);

		// set the input for the engine
		cpRecoEngine->SetInput(cpAudio, TRUE);
		hr = cpRecoEngine->SetRecoState(SPRST_ACTIVE);


		hr = cpRecoEngine->CreateRecoContext(&recoContext);
		checkResult(hr);

		initializeGrammar();

		hr = recoContext->SetNotifyWin32Event();
		checkResult(hr);

		HANDLE handleEvent;
		handleEvent = recoContext->GetNotifyEventHandle();
		if (handleEvent == INVALID_HANDLE_VALUE) 
		{
			checkResult(E_FAIL);
		}

		silentMode = false;
	}
}

SpeechRecognition::~SpeechRecognition()
{
	recoGrammar->Release();
	//::CoUninitialize();
}

// Fills vector with words if something is recognized
void SpeechRecognition::getCommand(std::vector<int>& voiceCommand)
{
	// Process all of the recognition events
	while (hr = srEvent.GetFrom(recoContext) == S_OK)
	{
		// Warning hr equal S_FALSE if everything is OK 
		// but eventCount < requestedEventCount
		if (!(hr == S_OK || hr == S_FALSE)) {
			checkResult(hr);
		}

		switch (srEvent.eEventId)
		{
		case SPEI_SOUND_START:
			m_bInSound = TRUE;
			break;

		case SPEI_SOUND_END:
			if (m_bInSound)
			{
				m_bInSound = FALSE;
			}
			break;
		
		// There may be multiple recognition results, so get all of them
		case SPEI_RECOGNITION:	
		{
			CSpDynamicString dstrText;

			hr = srEvent.RecoResult()->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, &dstrText, NULL);
			checkResult(hr);
			//words = splitString(dstrText.CopyToChar());

			SPPHRASE* phrase = NULL;
			hr = srEvent.RecoResult()->GetPhrase(&phrase);

			//if (0 == wcscmp(L"ruleListening", phrase->Rule.pszName))
			//{
			//	int command = phrase->pProperties->vValue.intVal;
			//	if (command == 1) // Start
			//	{
			//		silentMode = false;
			//	}
			//	else if (command == 2) // stop
			//	{
			//		silentMode = true;
			//	}
			//	voiceCommand.push_back(command);
			//}

			if (!silentMode)
			{
				if (0 == wcscmp(L"ruleProduce", phrase->Rule.pszName))
				{
					const SPPHRASEPROPERTY *number = phrase->pProperties;
					const SPPHRASEPROPERTY *unit = number->pNextSibling;

					voiceCommand.push_back(number->vValue.intVal);
					voiceCommand.push_back(unit->vValue.intVal);
				}
			}

		}
		break;

		}
	}
}

bool SpeechRecognition::switchSilentMode()
{
	bool r = !silentMode;
	silentMode = r;
	return r;
}