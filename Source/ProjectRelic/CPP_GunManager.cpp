// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_GunManager.h"

ACPP_GunManager::ACPP_GunManager()
	:m_ammoCount( 30 )
{
}

void ACPP_GunManager::SetAmmoCount( int ammoCount )
{
	m_ammoCount = ammoCount;
}

int ACPP_GunManager::GetAmmoCount()
{
	return m_ammoCount;
}
