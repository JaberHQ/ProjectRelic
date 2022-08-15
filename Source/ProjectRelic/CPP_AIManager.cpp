// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_AIManager.h"
#include "CPP_AIController.h"
#include "Engine/World.h"
#include "CPP_PlayerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Misc/App.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "CPP_AISettings.h"

ACPP_AIManager::ACPP_AIManager()
	:health( 100.0f )
	,defaultHealth( 100.0f )
	,m_canTakedown( true)
	,m_sightRadius( 1000.0f )
	,m_loseSightRadius( 1500.0f )
	,m_peripheralVisionAngleDegrees( 35.0f )
	,m_patrolSpeed( 300.0f )
	,m_chaseSpeed( 600.0f )
	,m_shotDamage( 23.3f )
	,m_deathTimer( 25.0f )
	,m_hasBeenSeen( false )
	,m_hasBeenCaught( false )
	,m_sightValuePercent( 0.0f )
	,m_curveFloat( 0.0f )
	,m_hasSeenSomething( false )
	,m_detectionSpeed( 0.0f )
	,m_headShotDamage( 2.0f )
	,soundHuh()
	,m_isInCover( false )
	,animDeath()
	,m_dead()
	,bottleTag( TEXT( "bottleNoise" ) )
	,m_hearingRange( 3000.0f )
	,m_hearingAge( 35.0f )
{
	PrimaryActorTick.bCanEverTick = true;

	// Initialise components
	perceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>( TEXT( "AIPerceptionComponent" ) );
	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>( TEXT( "SightConfig" ) );
	hearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>( TEXT( "HearingConfig" ) );
	boxComponent = CreateDefaultSubobject<UBoxComponent>( TEXT( "TakedownBox" ) );

	if( perceptionComp )
	{
		// Perception config
		perceptionComp->ConfigureSense( *sightConfig );
		perceptionComp->ConfigureSense( *hearingConfig );
		perceptionComp->SetDominantSense( sightConfig->GetSenseImplementation() );
	}


	if( sightConfig )
	{
		// Sight config
		sightConfig->SightRadius = m_sightRadius;
		sightConfig->LoseSightRadius = m_loseSightRadius;
		sightConfig->PeripheralVisionAngleDegrees = m_peripheralVisionAngleDegrees;
		sightConfig->DetectionByAffiliation.bDetectEnemies = true;
		sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		sightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	}

	if( hearingConfig )
	{
		// Hearing config
		hearingConfig->HearingRange = m_hearingRange;
		hearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		hearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
		hearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
		hearingConfig->SetMaxAge( m_hearingAge );
	}

	//perceptionComp->SetDominantSense( hearingConfig->GetSenseImplementation() );

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	m_assaultRifle = true;
}

void ACPP_AIManager::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	// Tick for sight evaluation
	SightValueTick();

	// Tick for if enemy can be seen through xray powerup
	XrayEvaluation();

	// Broadcast delegate for sight detection widget
	SightDetectionDelegate();

	
}

void ACPP_AIManager::BeginPlay()
{
	Super::BeginPlay();

	// If enemy 'senses' the player
	perceptionComp->OnPerceptionUpdated.AddDynamic( this, &ACPP_AIManager::OnUpdated );

	// Box component overlap
	boxComponent->OnComponentBeginOverlap.AddDynamic( this, &ACPP_AIManager::OnBoxBeginOverlap );
	boxComponent->OnComponentEndOverlap.AddDynamic( this, &ACPP_AIManager::OnBoxEndOverlap );


}

ACPP_PatrolPoint* ACPP_AIManager::GetPatrolPath()
{
	return m_patrolPath;
}

void ACPP_AIManager::OnBoxBeginOverlap( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult )
{
	// Player reference
	ACPP_PlayerManager* playerManager = Cast<ACPP_PlayerManager>( UGameplayStatics::GetPlayerPawn( GetWorld(), 0 ) );

	// Set bool to takedown
	playerManager->SetCanTakedown( true );
}

void ACPP_AIManager::OnBoxEndOverlap( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex )
{
	// -- IMPLEMENTATION NEEDED --
}

void ACPP_AIManager::Takedown()
{
	
	if( animTakedownAI )
	{
		// Play animation
		PlayAnimMontage( animTakedownAI );
	}

	// Stop AI from behaving
	GetController()->UnPossess();

	// Set collision
	SetActorEnableCollision( false );

	// Delay then death
	FTimerHandle deathDelayTimer;
	GetWorld()->GetTimerManager().SetTimer( deathDelayTimer, this, &ACPP_AIManager::DelayDeath, m_deathTimer, false );
}

bool ACPP_AIManager::GetInCover()
{
	return m_isInCover;
}

void ACPP_AIManager::DelayDeath()
{
	// Destroy Actor
	Destroy();
}

void ACPP_AIManager::TakeAttack()
{
	// Temporary damage variable
	float damage = m_shotDamage;

	// If enemy has been shot in the head
	if( m_shotInHead )
	{
		// Increase damage value
		damage = m_shotDamage * m_headShotDamage;
	}
	
	// If health remains, decrease health else pronounce Enemy Dead
	health >= 0.0f ? health -= damage : m_dead = true; // Change to death animation
	
	// AI Controller reference
	ACPP_AIController* controllerAI = Cast<ACPP_AIController>( GetController() );

	// Player reference
	ACPP_PlayerManager* playerManager = Cast<ACPP_PlayerManager>( UGameplayStatics::GetPlayerPawn( GetWorld(), 0 ) );

	if( controllerAI )
	{
		// Set player caught
		controllerAI->PlayerHasShot();

		// Set sight value to full
		SetSightValuePercent( 1.0f );

		if( playerManager )
		{
			// Set hitmarker if hit but not dead

			health > 0.0f ? playerManager->SetHitmarkerActive( true ) : playerManager->SetDeathHitmarkerActive( true );
			if( health > 0.0f )
			{
				playerManager->SetHitmarkerActive( true );
			}
			// Set death hitmarker if dead
			else
			{
				playerManager->SetDeathHitmarkerActive( true );
				m_dead = true;
			}
			
		}
		if( m_dead )
		{
			controllerAI->SetDead( true );
			// Disable movement
			GetCharacterMovement()->DisableMovement();

			//Unpossess
			UnPossessed();

			// Set collision
			SetActorEnableCollision( false );

			PlayAnimMontage( animDeath );
		}
	}

	// Enemy has been shot
	m_hasBeenShot = true;

	// Reset shot to the head bool
	m_shotInHead = false;
}

float ACPP_AIManager::DetectionSpeedCalculation()
{
	// AI Controller reference
	ACPP_AIController* controllerAI = Cast<ACPP_AIController>( GetController() );

	// Player reference
	ACPP_PlayerManager* playerManager = Cast<ACPP_PlayerManager>( UGameplayStatics::GetPlayerPawn( GetWorld(), 0 ) );

	/*if( controllerAI )
	{
		if( playerManager )
		{
			FVector playerLocation = playerManager->GetActorLocation();
			FVector enemyLocation = controllerAI->GetPawn()->GetActorLocation();
			float distance = FVector::Distance( playerLocation, enemyLocation );
			float speed = UKismetMathLibrary::NormalizeToRange( distance, 0.0f, 500.0f );
			float detectionSpeed = myCurve->GetFloatValue( speed );
			return detectionSpeed;
		}
	}*/
	return 0.0f;
}

void ACPP_AIManager::DelayInvestigate()
{
}

bool ACPP_AIManager::GetHasCaughtPlayer()
{
	return m_hasBeenCaught;
}


void ACPP_AIManager::SetupPerceptionSystem()
{
}

void ACPP_AIManager::EnterCover()
{
	m_isInCover = true;
	Crouch();
}

void ACPP_AIManager::ShootPlayer()
{
	// Shoot
	ShootProjectile();

	// Set shooting timer
	GetWorld()->GetTimerManager().SetTimer( m_stopShooting, this, &ACPP_AIManager::StopShootingPlayer, 5.0f, true );

}

void ACPP_AIManager::StopShootingPlayer()
{
	// Clear timers
	GetWorld()->GetTimerManager().ClearTimer( m_shootTime );
	GetWorld()->GetTimerManager().ClearTimer( m_stopShooting );
}

void ACPP_AIManager::SightValueTick()
{
	// Set curve for detection speed
	m_detectionSpeed = myCurve->GetFloatValue( m_curveFloat );

	// Set sight value percent
	m_sightValuePercent = UKismetMathLibrary::FInterpTo_Constant( m_sightValuePercent, UKismetMathLibrary::SelectFloat( 1.0f, 0.0f, m_hasSeenSomething ),
		FApp::GetDeltaTime(), m_detectionSpeed );

	// If sight value is full
	if( m_sightValuePercent >= 1.0f )
	{
		// Set player has been caught
		m_hasBeenCaught = true;

		// Cast to AI controller
		ACPP_AIController* controllerAI = Cast<ACPP_AIController>( GetController() );

		if( controllerAI )
		{
			// Cast to player
			ACPP_PlayerManager* playerManager = Cast<ACPP_PlayerManager>( UGameplayStatics::GetPlayerPawn( GetWorld(), 0 ) );

			if( playerManager )
			{
				// Set Player and AI location
				FVector playerLocation = playerManager->GetActorLocation();
				FVector enemyLocation = controllerAI->GetPawn()->GetActorLocation();

				// Find the distance between the two
				float distance = FVector::Distance( playerLocation, enemyLocation );

				// Set curve float
				m_curveFloat = UKismetMathLibrary::NormalizeToRange( distance, 0.0f, 1000.0f );

				// Set has line of sight
				controllerAI->SetHasLineOfSight( true );
			}
		}
	}
}

void ACPP_AIManager::XrayEvaluation()
{
	// Player reference
	ACPP_PlayerManager* playerManager = Cast<ACPP_PlayerManager>( UGameplayStatics::GetPlayerPawn( GetWorld(), 0 ) );
	if( playerManager )
	{
		// If player is using xray vision
		if( playerManager->GetXray() )
		{
			// Set enemy to be seen through walls
			GetMesh()->SetRenderCustomDepth( true );
		}
		else
		{
			// Stop enemy to be seen through walls
			GetMesh()->SetRenderCustomDepth( false );

		}
	}
}

void ACPP_AIManager::TimeToShoot()
{
	// Set shoot timer
	GetWorld()->GetTimerManager().SetTimer( m_shootTime, this, &ACPP_AIManager::ShootPlayer, timeBetweenShots, true );

	// Crouch
	Crouch();
}

void ACPP_AIManager::SetHasCaughtPlayer( bool boolean )
{
	// Set bool
	m_hasBeenCaught = boolean;
}

void ACPP_AIManager::SetMaxWalkSpeed( float speed )
{
	// Set max walk speed
	GetCharacterMovement()->MaxWalkSpeed = speed;
}

void ACPP_AIManager::SetSightValuePercent( float sightValuePercent )
{
	m_sightValuePercent = sightValuePercent;
}

bool ACPP_AIManager::GetCanTakedown()
{
	return m_canTakedown;
}

void ACPP_AIManager::OnUpdated( const TArray<AActor*>& caughtActors )
{
	// AI Controller reference
	ACPP_AIController* controllerAI = Cast<ACPP_AIController>( GetController() );

	// Player reference
	ACPP_PlayerManager* playerManager = Cast<ACPP_PlayerManager>( UGameplayStatics::GetPlayerPawn( GetWorld(), 0 ) );

	if( controllerAI )
	{
		if( playerManager )
		{
			if( playerManager->GetInvisibilityStatus() == false )
			{
				// Register pecepetion 
				UAIPerceptionSystem::RegisterPerceptionStimuliSource( this, sightConfig->GetSenseImplementation(), controllerAI );

				for( int i = 0; i < caughtActors.Num(); i++ )
				{
					// Actor perception
					FActorPerceptionBlueprintInfo info;
					perceptionComp->GetActorsPerception( caughtActors[ i ], info );

					for( int j = 0; j < info.LastSensedStimuli.Num(); j++ )
					{
						FAIStimulus stimulus = info.LastSensedStimuli[ j ];
						if( stimulus.WasSuccessfullySensed() )
						{
							// If player
							if( caughtActors[ i ] == playerManager )
							{
								// If sense
								if( stimulus.Type.Name == "Default__AISense_sight" )
								{

									m_hasSeenSomething = true;

									// Get location
									FVector playerLocation = perceptionComp->GetActorInfo( *caughtActors[ i ] )->GetStimulusLocation( sightConfig->GetSenseID() );
									FVector enemyLocation = perceptionComp->GetActorInfo( *caughtActors[ i ] )->GetReceiverLocation( sightConfig->GetSenseID() );

									// Distance between Player and Enemy
									float distance = FVector::Distance( playerLocation, enemyLocation );

									// Set last known location of the Player
									controllerAI->SetLastKnownLocation( playerLocation );

									//Investigate
									controllerAI->SetInvestigate( true );

								}
								// If noise is detected
								if( stimulus.Tag == noiseTag )
								{
									FVector shotLocation = perceptionComp->GetActorInfo( *caughtActors[ i ] )->GetLastStimulusLocation();
									controllerAI->SetLastKnownLocation( shotLocation );
									//Investigate
									controllerAI->SetInvestigate( true );
								}
							}

							
						}
							
											
					}
				}
			}
			// If player has been caught
			if( m_hasBeenCaught )
			{
				// Set actor (Player) as caught
				controllerAI->PlayerHasShot();
			}

		}
	}
	
}


void ACPP_AIManager::OnPlayerCaught( const TArray<AActor*>& caughtActors )
{
	// AI Controller reference
	ACPP_AIController* controllerAI = Cast<ACPP_AIController>( GetController() );

	// Player reference
	ACPP_PlayerManager* playerManager = Cast<ACPP_PlayerManager>( UGameplayStatics::GetPlayerPawn( GetWorld(), 0 ) );

	if( controllerAI )
	{
		if( playerManager )
		{
			if( playerManager->GetInvisibilityStatus() == false )
			{
				// Set bool to investigate
				m_hasSeenSomething = true;

				// Sight config
				UAIPerceptionSystem::RegisterPerceptionStimuliSource( this, sightConfig->GetSenseImplementation(), controllerAI );

				// Get location
				FVector playerLocation = perceptionComp->GetActorInfo( *caughtActors[ 0 ] )->GetStimulusLocation( sightConfig->GetSenseID() );
				FVector enemyLocation = perceptionComp->GetActorInfo( *caughtActors[ 0 ] )->GetReceiverLocation( sightConfig->GetSenseID() );

				// Distance between Player and Enemy
				float distance = FVector::Distance( playerLocation, enemyLocation );

				// Set last known location of the Player
				controllerAI->SetLastKnownLocation( playerLocation );

				//Investigate
				controllerAI->SetInvestigate( true );

				// Actor perception
				FActorPerceptionBlueprintInfo info;
				perceptionComp->GetActorsPerception( playerManager, info );

				// If sensed
				if( info.LastSensedStimuli.Num() > 0 )
				{
					FAIStimulus stimulus = info.LastSensedStimuli[ 0 ];
				
					if( stimulus.WasSuccessfullySensed() )
					{
						// Set bool to investigate
						m_hasSeenSomething = true;
						//UGameplayStatics::PlaySoundAtLocation( GetWorld(), soundHuh, GetActorLocation(), 0.3f );
					}
					else
					{
						// Go back to regular pathing
						if( m_hasBeenCaught == false )
						{
							m_hasSeenSomething = false;
						}
					}
				}
			}
			
			// If player has been caught
			if( m_hasBeenCaught )
			{
				// Set actor (Player) as caught
				controllerAI->PlayerHasShot();
			}	
		}
	}
}

void ACPP_AIManager::SightDetectionDelegate()
{
	// Broadcast delegate
	sightDetectionD.Broadcast( m_sightValuePercent, m_hasSeenSomething );
}

void ACPP_AIManager::EvaluateSightDetection()
{
	// If detection meter is empty
	if( m_sightValuePercent <= 0.0f && !m_hasSeenSomething )
	{
		// Lose player and go back to regular pathing
		GiveUp();
		LostPlayer();
	}
	else
	{
		if( m_sightValuePercent >= 1.0f )
		{
			// Attack player
			SeenPlayer();
		}
	}
	
}

void ACPP_AIManager::GiveUp()
{
}

void ACPP_AIManager::SeenPlayer()
{
}

void ACPP_AIManager::LostPlayer()
{
}




