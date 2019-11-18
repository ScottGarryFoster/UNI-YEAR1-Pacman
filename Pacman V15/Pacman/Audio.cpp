#include "Pacman.h"

#include <sstream>
#include <array>
#include <string>
#include <iostream>

using namespace S2D;

bool Pacman::AudioLoadContent()
{
	m_seMusicSound = new SoundEffect();
	for (int i = 0; i < 10; i++)
		m_seSoundEffects[i].seSoundEffect = new SoundEffect();
		

	//Defaults for Sound Effects:
	//m_seChopSound = new SoundEffect();
	//m_seChopSound->Load("MusicAndSounds/COPYRIGHTED/singleChomp.mp3");
	return true;
}

bool Pacman::Audio_Update()
{
	if (m_bPlayPacmanDeath)
	{
		if (m_bGhostAI_SpecialBitActive)		//Stop the Scatter Phase and stop the music
		{
			m_bGhostAI_SpecialBitActive = false;//Used for when the special bit is picked up
			m_bGhostAI_SpecialBitClose = false;//For when the Special phase is almost over
			m_iNextScatterTime = -1;
			m_iTimesScattered = -1;
		}
		m_iCollectableAffectSeconds = -1;

		if (m_iLevelNumber > 2) m_iMapNumber = -1;//Makes it so the map is random

		Audio_PlaySoundEffect(2, true);
		Audio_StopMusic();
	}
		
	m_bPlayPacmanDeath = false;
	return true;
}
bool Pacman::Audio_SetMusic(int iTrack, bool bSetup)
{
	switch (iTrack)
	{
	case 10:
		if (bSetup)
			m_seMusicSound->Load("MusicAndSounds/Original - Online Sequence/Pacman-GhostSound.wav");
		m_iMusicLengthInSeconds = 23; //Four Seconds long
		break;
	case 1:
	default:
		if (bSetup)
			m_seMusicSound->Load("MusicAndSounds/Original - Online Sequence/Pacman-intro.wav");
		m_iMusicLengthInSeconds = 5; //Four Seconds long
		break;
	}
	m_iMusicLengthInSecondsElap = 0; //Used to look into the future
	m_bMusicIsPlaying = false;
	return true;
}

bool Pacman::Audio_PlayMusic()
{
	if (!m_bMusicIsPlaying)
	{
		m_iMusicLengthInSecondsElap = UtilityTimeInTheFuture(m_iMusicLengthInSeconds);
		Audio::Play(m_seMusicSound);
		m_bMusicIsPlaying = true;
	}

	if (UtilityReturnSeconds() > m_iMusicLengthInSecondsElap)//The song has finished playing
	{
		Audio::Stop(m_seMusicSound);
		m_bMusicIsPlaying = false;
	}
	return m_bMusicIsPlaying;
}

bool Pacman::Audio_PauseMusic()
{
	m_iMusicTimeLeftOver = m_iMusicLengthInSecondsElap - UtilityReturnSeconds();
	Audio::Pause(m_seMusicSound);
	return true;
}

bool Pacman::Audio_ResumeMusic()
{
	m_iMusicLengthInSecondsElap = UtilityTimeInTheFuture(m_iMusicTimeLeftOver);
	m_iMusicTimeLeftOver = -1;
	Audio::Resume(m_seMusicSound);
	return true;
}

bool Pacman::Audio_StopMusic()
{
	Audio::Stop(m_seMusicSound);
	m_bMusicIsPlaying = false;
	return true;
}

bool Pacman::Audio_SetSoundEffect(int iSoundEffectID, int iSoundEffectToLoad)
{
	if (iSoundEffectID < 0)
	{
		cout << "Audio_SetSoundEffect: iSoundEffectID is lower than 0" << endl;
		return false;
	}
	else if (iSoundEffectID > 9)
	{
		cout << "Audio_SetSoundEffect: iSoundEffectID is greater than 9" << endl;
		return false;
	}
	switch (iSoundEffectToLoad)
	{
	case 11:
		m_seSoundEffects[iSoundEffectID].seSoundEffect->Load("MusicAndSounds/InventorySFX/Select1.wav");
		m_seSoundEffects[iSoundEffectID].seSoundEffectsSecondsElap = 0;
		m_seSoundEffects[iSoundEffectID].seSoundEffectsLengthInSeconds = 1050;
		m_seSoundEffects[iSoundEffectID].bSoundEffectIsPlaying = false;
		break;
	case 12:
		m_seSoundEffects[iSoundEffectID].seSoundEffect->Load("MusicAndSounds/InventorySFX/Select2.wav");
		m_seSoundEffects[iSoundEffectID].seSoundEffectsSecondsElap = 0;
		m_seSoundEffects[iSoundEffectID].seSoundEffectsLengthInSeconds = 1200;
		m_seSoundEffects[iSoundEffectID].bSoundEffectIsPlaying = false;
		break;
	case 13:
		m_seSoundEffects[iSoundEffectID].seSoundEffect->Load("MusicAndSounds/InventorySFX/Select3.wav");
		m_seSoundEffects[iSoundEffectID].seSoundEffectsSecondsElap = 0;
		m_seSoundEffects[iSoundEffectID].seSoundEffectsLengthInSeconds = 400;
		m_seSoundEffects[iSoundEffectID].bSoundEffectIsPlaying = false;
		break;
	case 14:
		m_seSoundEffects[iSoundEffectID].seSoundEffect->Load("MusicAndSounds/InventorySFX/Select4.wav");
		m_seSoundEffects[iSoundEffectID].seSoundEffectsSecondsElap = 0;
		m_seSoundEffects[iSoundEffectID].seSoundEffectsLengthInSeconds = 864;
		m_seSoundEffects[iSoundEffectID].bSoundEffectIsPlaying = false;
		break;
	case 15:
		m_seSoundEffects[iSoundEffectID].seSoundEffect->Load("MusicAndSounds/InventorySFX/Select5.wav");
		m_seSoundEffects[iSoundEffectID].seSoundEffectsSecondsElap = 0;
		m_seSoundEffects[iSoundEffectID].seSoundEffectsLengthInSeconds = 650;
		m_seSoundEffects[iSoundEffectID].bSoundEffectIsPlaying = false;
		break;
	case 16:
		m_seSoundEffects[iSoundEffectID].seSoundEffect->Load("MusicAndSounds/InventorySFX/Select6.wav");
		m_seSoundEffects[iSoundEffectID].seSoundEffectsSecondsElap = 0;
		m_seSoundEffects[iSoundEffectID].seSoundEffectsLengthInSeconds = 864;
		m_seSoundEffects[iSoundEffectID].bSoundEffectIsPlaying = false;
		break;
	case 17:
		m_seSoundEffects[iSoundEffectID].seSoundEffect->Load("MusicAndSounds/InventorySFX/Select7.wav");
		m_seSoundEffects[iSoundEffectID].seSoundEffectsSecondsElap = 0;
		m_seSoundEffects[iSoundEffectID].seSoundEffectsLengthInSeconds = 648;
		m_seSoundEffects[iSoundEffectID].bSoundEffectIsPlaying = false;
		break;
	case 18:
		m_seSoundEffects[iSoundEffectID].seSoundEffect->Load("MusicAndSounds/InventorySFX/Select8.wav");
		m_seSoundEffects[iSoundEffectID].seSoundEffectsSecondsElap = 0;
		m_seSoundEffects[iSoundEffectID].seSoundEffectsLengthInSeconds = 864;
		m_seSoundEffects[iSoundEffectID].bSoundEffectIsPlaying = false;
		break;
	case 19:
		m_seSoundEffects[iSoundEffectID].seSoundEffect->Load("MusicAndSounds/InventorySFX/Select9.wav");
		m_seSoundEffects[iSoundEffectID].seSoundEffectsSecondsElap = 0;
		m_seSoundEffects[iSoundEffectID].seSoundEffectsLengthInSeconds = 1056;
		m_seSoundEffects[iSoundEffectID].bSoundEffectIsPlaying = false;
		break;
	case 3:
		m_seSoundEffects[iSoundEffectID].seSoundEffect->Load("MusicAndSounds/InventorySFX/LevelUp3.wav");
		m_seSoundEffects[iSoundEffectID].seSoundEffectsSecondsElap = 0;
		m_seSoundEffects[iSoundEffectID].seSoundEffectsLengthInSeconds = 1776;
		m_seSoundEffects[iSoundEffectID].bSoundEffectIsPlaying = false;
		break;
	case 2:
		m_seSoundEffects[iSoundEffectID].seSoundEffect->Load("MusicAndSounds/Original - Online Sequence/Pacman-Death.wav");
		m_seSoundEffects[iSoundEffectID].seSoundEffectsSecondsElap = 0;
		m_seSoundEffects[iSoundEffectID].seSoundEffectsLengthInSeconds = 1000;
		m_seSoundEffects[iSoundEffectID].bSoundEffectIsPlaying = false;
		break;
	case 1:
	default:
		m_seSoundEffects[iSoundEffectID].seSoundEffect->Load("MusicAndSounds/Original - Online Sequence/Pacman-Collect_Short.wav");
		m_seSoundEffects[iSoundEffectID].seSoundEffectsSecondsElap = 0;
		m_seSoundEffects[iSoundEffectID].seSoundEffectsLengthInSeconds = 310;
		m_seSoundEffects[iSoundEffectID].bSoundEffectIsPlaying = false;
		break;
	}
	return true;
}

bool Pacman::Audio_PlaySoundEffect(int iSoundEffectID, bool bInterupt)
{
	soundEffect* seSoundEffect = &m_seSoundEffects[iSoundEffectID];

	if (seSoundEffect->seSoundEffect->GetGain() != m_fOptionsVolumeSoundEffects)
		seSoundEffect->seSoundEffect->SetGain(m_fOptionsVolumeSoundEffects);

	if (bInterupt)
	{
		//cout << "Interupt" << endl;
		seSoundEffect->bSoundEffectIsPlaying = false;
		Audio::Stop(seSoundEffect->seSoundEffect);
	}

	if (UtilityReturnSeconds(true) >= seSoundEffect->seSoundEffectsSecondsElap)//The song has finished playing
	{
		//cout << "Still running" << endl;
		Audio::Stop(seSoundEffect->seSoundEffect);
		seSoundEffect->bSoundEffectIsPlaying = false;
	}

	if (!seSoundEffect->bSoundEffectIsPlaying)
	{
		seSoundEffect->seSoundEffectsSecondsElap = UtilityTimeInTheFuture(seSoundEffect->seSoundEffectsLengthInSeconds, true);
		Audio::Play(seSoundEffect->seSoundEffect);
		seSoundEffect->bSoundEffectIsPlaying = true;
		//cout << "In Playing" << endl;
	}

	if (UtilityReturnSeconds(true) >= seSoundEffect->seSoundEffectsSecondsElap)//The song has finished playing
	{
		//cout << "Still running" << endl;
		Audio::Stop(seSoundEffect->seSoundEffect);
		seSoundEffect->bSoundEffectIsPlaying = false;
	}
	//cout << "1" << endl;
	return seSoundEffect->bSoundEffectIsPlaying;
}

bool Pacman::Audio_UpdateHasSoundPlaying(int iSoundEffectID)
{
	soundEffect* seSoundEffect = &m_seSoundEffects[iSoundEffectID];

	if (seSoundEffect->seSoundEffectsSecondsElap == 0)
		return false;

	if (UtilityReturnSeconds(true) >= seSoundEffect->seSoundEffectsSecondsElap)//The song has finished playing
	{
		seSoundEffect->bSoundEffectIsPlaying = false;
		return false;
	}
	seSoundEffect->bSoundEffectIsPlaying = true;
	//cout << "TRUE" << endl;
	return true;
}