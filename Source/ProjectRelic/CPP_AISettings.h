// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PROJECTRELIC_API CPP_AISettings
{
private:

public:
	CPP_AISettings();
	~CPP_AISettings();
	bool m_playerHasBeenCaught;

	void SetPlayerCaught( bool boolean );
	bool GetPlayerCaught();
};
