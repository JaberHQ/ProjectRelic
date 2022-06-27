// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_WeaponManager.h"
#include "CPP_GunManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRELIC_API ACPP_GunManager : public ACPP_WeaponManager
{
	GENERATED_BODY()
private:
	int m_ammoCount;
	int m_ammoRemaining;
public:
	ACPP_GunManager();
	void SetTotalAmmoCount( int ammoCount );
	int GetTotalAmmoCount();

	void SetAmmoRemaining( int ammoRemaining );
	int GetAmmoRemaining();

};
