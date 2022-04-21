// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EnemyController.h"

//#include "Engine.h".

AEnemyCharacter::AEnemyCharacter()
	:m_health( 100.0f )
	,m_detectionTimer( 0.0f )	
	,m_sightRadius( 1000.0f )
	,m_loseSightRadius( 1020.0f )
	,m_peripheralVisionAngleDegrees( 35.0f )
	,m_patrolSpeed( 300.0f )
	,m_chaseSpeed( 600.0f )
	,m_canTakedown( true )
	//,m_pCombatInterface( nullptr )
{
	// Set default walk speed
	UpdateWalkSpeed( m_patrolSpeed );

	// Initialise components
	perceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>( TEXT( "AIPerception Component" ) );
	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>( TEXT( "Sight Config" ) );
	/*widgetComp = CreateDefaultSubobject<UWidgetComponent>( TEXT( "Widget Component" ) );*/
	// Perception config
	perceptionComp->ConfigureSense( *sightConfig );
	perceptionComp->SetDominantSense( sightConfig->GetSenseImplementation() );

	

	// Sight config
	sightConfig->SightRadius = m_sightRadius;
	sightConfig->LoseSightRadius = m_loseSightRadius;
	sightConfig->PeripheralVisionAngleDegrees = m_peripheralVisionAngleDegrees;
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = false;

}

AEnemyCharacter::~AEnemyCharacter()
{	
	//delete[] m_combatInterface;
	//m_combatInterface = nullptr;
}



void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// If enemy 'senses' the player
	perceptionComp->OnPerceptionUpdated.AddDynamic( this, &AEnemyCharacter::OnPlayerCaught );
}

AEnemyCharacter* AEnemyCharacter::GetEnemyCharacter( APawn* pawn ) const
{
	AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>( pawn );
	return enemyCharacter;
}

bool AEnemyCharacter::CanTakedown()
{
	return m_canTakedown;
}

void AEnemyCharacter::Interact()
{
	UE_LOG( LogTemp, Warning, TEXT( "Implementation" ) );
}



void AEnemyCharacter::OnPlayerCaught( const TArray<AActor*>& caughtActors )
{
	// Reference to player controller
	AEnemyController* enemyController = Cast<AEnemyController>( GetController() );

	if( enemyController )
	{	
		// Debug message
		GEngine->AddOnScreenDebugMessage( -1, 5.0f, FColor::Red, ( TEXT( "Pew" ) ) );

		// Set actor (player) as caught
		enemyController->SetPlayerCaught( caughtActors );

		// Shoot at player
		Shoot();

		// Sight config
		UAIPerceptionSystem::RegisterPerceptionStimuliSource( this, sightConfig->GetSenseImplementation(), enemyController );
		
		// Speed up enemy to sprint
		UpdateWalkSpeed( m_chaseSpeed );				
	}			
}



