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
public:
	ACPP_GunManager();
	void SetAmmoCount( int ammoCount );
	int GetAmmoCount();

};
