// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_AmmoPickup.h"
#include "CPP_PlayerManager.h"
void ACPP_AmmoPickup::BeginPlay()
{
	boxComp->OnComponentBeginOverlap.AddDynamic( this, &ACPP_Pickup::OnBoxBeginOverlap );
}

void ACPP_AmmoPickup::OnBoxBeginOverlap( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult )
{
	ACPP_PlayerManager* playerManager = Cast<ACPP_PlayerManager>( UGameplayStatics::GetPlayerCharacter( GetWorld(), 0 ) );
	if( playerManager )
	{
		playerManager->IncreaseAmmoCount( 30 );
		cubeComp->DestroyComponent();
	}

	
}

ACPP_AmmoPickup::ACPP_AmmoPickup()
{
}
