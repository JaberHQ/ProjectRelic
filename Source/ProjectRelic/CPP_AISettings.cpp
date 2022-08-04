// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_AISettings.h"

CPP_AISettings::CPP_AISettings()
	:m_playerHasBeenCaught( false )
{
	
}

CPP_AISettings::~CPP_AISettings()
{
}

void CPP_AISettings::SetPlayerCaught( bool boolean )
{
	m_playerHasBeenCaught = boolean;
}

bool CPP_AISettings::GetPlayerCaught()
{
	return m_playerHasBeenCaught;
}
