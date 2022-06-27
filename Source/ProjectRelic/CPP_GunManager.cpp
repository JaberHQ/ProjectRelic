// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_GunManager.h"

ACPP_GunManager::ACPP_GunManager()
	:m_ammoCount( 30 )
	,m_ammoRemaining( 30 )
{
	
}

void ACPP_GunManager::SetTotalAmmoCount( int ammoCount )
{
	m_ammoCount = ammoCount;
}

int ACPP_GunManager::GetTotalAmmoCount()
{
	return m_ammoCount;
}

void ACPP_GunManager::SetAmmoRemaining( int ammoRemaining )
{
	m_ammoRemaining = ammoRemaining;
}

int ACPP_GunManager::GetAmmoRemaining()
{
	return m_ammoRemaining;
}
