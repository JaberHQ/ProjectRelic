// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

#include "CPP_AIController.generated.h"
/*********************************************************************************************************************************************************
 * Type: Class
 *
 * Name: CPP_AIController
 *
 * Author: Jaber Ahmed
 *
 * Purpose: AI Controller to control Enemy Behaviour
 *
 * References: Unreal Engine, [Online] https://www.youtube.com/watch?v=iY1jnFvHgbE&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=3 [Accessed 02/05/2022],
 *			   Reubs, [Online] https://www.youtube.com/watch?v=3Z1A825gTA8&list=PLBBe1hvULrciqnr3wdS77c5CLfTeHza0X&index=4 [Accessed 02/05/2022],
 *			   	
 *
 * See Also: CPP_AIManager.h
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 02/05/2022    JA          v2.0        Create a better EnemyController
 * 13/06/2022    JA		     v2.1		 Investigating
 * 13/06/2022	 JA		     v2.2		 Detection meter
 * 15/07/2022	 JA			 v2.3		 Improving AI functionality
 * 04/08/2022	 JA			 v2.4		 Created a cover system
*********************************************************************************************************************************************************/
UCLASS()
class PROJECTRELIC_API ACPP_AIController : public AAIController
{
	GENERATED_BODY()

private:
	UBehaviorTreeComponent* behaviourComp; // Behaviour Tree comp
	UBlackboardComponent* blackboardComp; // Blackboard Comp

	// BB keys
	UPROPERTY( EditDefaultsOnly, Category = "AI" )
		FName m_patrolLocation; // Patrol location

	UPROPERTY( EditDefaultsOnly, Category = "AI" )
		FName m_enemyActor; // Target

	UPROPERTY( EditDefaultsOnly, Category = "AI" )
		FName m_hasLineOfSight; // Has line of sight bool

	UPROPERTY( EditDefaultsOnly, Category = "AI" )
		FName m_patrolPathVector; // Patrol path vector

	UPROPERTY( EditDefaultsOnly, Category = "AI" )
		FName m_patrolPathIndex; // Patrol path index

	UPROPERTY( EditDefaultsOnly, Category = "AI" )
		FName m_lastKnownLocation; // Patrol path index
	
	UPROPERTY( EditDefaultsOnly, Category = "AI" )
		FName m_invesigate; // Patrol path index

	UPROPERTY( EditDefaultsOnly, Category = "AI" )
		FName m_move; // Move to cover

	UPROPERTY( EditDefaultsOnly, Category = "AI" )
		FName m_inCover; // In cover

	UPROPERTY( EditDefaultsOnly, Category = "AI" )
		FName m_bottleLocation; // Location of the projectile

	UPROPERTY( EditDefaultsOnly, Category = "AI" )
		FName m_dead; // If AI is dead

	int m_changeCoverNumber;
private:
	/**********************************************************************
	   *   Function        : virtual void OnPossess( APawn* pawn ) override
	   *   Purpose         : Possess pawn and control behaviour
	   *   Parameters      : APawn* pawn
	   *   Returns         : N/A
	   *   Date altered    : 09/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	***********************************************************************/
	virtual void OnPossess( APawn* pawn ) override;

public:
	/*****************************************************************************
	*   Function        : ACPP_AIController()
	*   Purpose         : Constructor
	*   Parameters      : N/A
	*   Returns         : N/A
	*   Date altered    : 02/05/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	*****************************************************************************/
	ACPP_AIController();
	/**********************************************************************************
	*   Function        : FORCEINLINE UBlackboardComponent* GetBlackboardComp() const
	*   Purpose         : Get Blackboard component
	*   Parameters      : N/A
	*   Returns         : blackboardComp
	*   Date altered    : 02/05/2022 
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A
	**********************************************************************************/
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const;
	/***********************************************************************************
	   *   Function        : void SetPlayerCaught( const TArray<AActor*>& caughtActors )
	   *   Purpose         : Set the target to the player when spotted
	   *   Parameters      : const TArray<AActor*>& caughtActors
	   *   Returns         : N/A
	   *   Date altered    : 09/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	***********************************************************************************/
	void SetPlayerCaught( const TArray<AActor*>& caughtActors );
	/*****************************************************************************
		*   Function        : void SetHasLineOfSight( bool hasLineOfSight )
		*   Purpose         : Set BB key boolean
		*   Parameters      : bool boolean
		*   Returns         : N/A
		*   Date altered    : 09/04/2022
		*   Contributors    : Jaber Ahmed
		*   Notes           : N/A
		*   See also        : N/A
	*****************************************************************************/
	void SetHasLineOfSight( bool boolean );
	/****************************************************************************************
		*   Function        : bool GetHasLineOfSight()
		*   Purpose         : Get BB key hasLineOfSight
		*   Parameters      : N/A
		*   Returns         : bool return blackboardComp->GetValueAsBool( m_hasLineOfSight )
		*   Date altered    : 09/04/2022
		*   Contributors    : Jaber Ahmed
		*   Notes           : N/A
		*   See also        : N/A
	****************************************************************************************/
	bool GetHasLineOfSight();
	/***********************************************************************************
	*   Function        : void SetLastKnownLocation( FVector vector )
	*   Purpose         : Set last known location of the Player
	*   Parameters      : FVector vector
	*   Returns         : N/A 
	*   Date altered    : 15/07/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : N/A           
	***********************************************************************************/
	void SetLastKnownLocation( FVector vector );
	 /*****************************************************************************
	 *   Function        : void SetInvestigate( bool boolean )
	 *   Purpose         : Set enemy to start or stop investigating 
	 *   Parameters      : bool boolean 
	 *   Returns         : N/A
	 *   Date altered    : 15/07/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	 *****************************************************************************/
	void SetInvestigate( bool boolean );
	/*************************************************************************************
	 *   Function        : FVector GetLastKnownLocation()
	 *   Purpose         : Get last known location of Player
	 *   Parameters      : N/A
	 *   Returns         : return blackboardComp->GetValueAsVector( m_lastKnownLocation )
	 *   Date altered    : 15/07/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	 *************************************************************************************/
	FVector GetLastKnownLocation();
	/*****************************************************************************
	 *   Function        : void PlayerHasShot()
	 *   Purpose         : Set player to be caught 
	 *   Parameters      : N/A
	 *   Returns         : N/A
	 *   Date altered    : 15/07/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	 *****************************************************************************/
	void PlayerHasShot();
	/*****************************************************************************
	 *   Function        : FVector GetMove()
	 *   Purpose         : Get move location from BB
	 *   Parameters      : N/A
	 *   Returns         : blackboardComp->GetValueAsVector( m_move )
	 *   Date altered    : 15/07/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : N/A
	 *****************************************************************************/
	FVector GetMove();
	/*****************************************************************************
	 *   Function        : void SetInCover( bool boolean )
	 *   Purpose         : Set player to be in cover
	 *   Parameters      : bool boolean
	 *   Returns         : N/A
	 *   Date altered    : 15/07/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : Behaviour Tree - BT_AssaultSoldier 
	 *****************************************************************************/
	void SetInCover( bool boolean );
	/*****************************************************************************
	 *   Function        : bool GetInCover( ) const;
	 *   Purpose         : Get player to be in cover
	 *   Parameters      : N/A
	 *   Returns         : return blackboardComp->GetValueAsBool( m_inCover );
	 *   Date altered    : 15/07/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : Behaviour Tree - BT_AssaultSoldier
	 *****************************************************************************/
	bool GetInCover() const;
	/*****************************************************************************
	 *   Function        : void SetBottleLocation( FVector bottleLocation )
	 *   Purpose         : Set bottle location
	 *   Parameters      : FVector bottleLocation 
	 *   Returns         : N/A
	 *   Date altered    : 04/08/2022
	 *   Contributors    : Jaber Ahmed
	 *   Notes           : N/A
	 *   See also        : Behaviour Tree - BT_AssaultSoldier
	 *****************************************************************************/
	UFUNCTION( BlueprintCallable )
		void SetBottleLocation( FVector bottleLocation );
	/*********************************************************************************
	*   Function        : FVector GetBottleLocation()
	*   Purpose         : Get bottle location
	*   Parameters      : N/A
	*   Returns         : return blackboardComp->GetValueAsVector( m_bottleLocation )
	*   Date altered    : 04/08/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : Behaviour Tree - BT_AssaultSoldier
	*********************************************************************************/
	FVector GetBottleLocation();
	/*********************************************************************************
	*   Function        : void SetDead( bool boolean )
	*   Purpose         : Set if AI is dead
	*   Parameters      : bool boolean
	*   Returns         : N/A
	*   Date altered    : 04/08/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : Behaviour Tree - BT_AssaultSoldier
	*********************************************************************************/
	void SetDead( bool boolean );
	/*********************************************************************************
	*   Function        : bool GetDead
	*   Purpose         : Get dead bool
	*   Parameters      : N/A
	*   Returns         : return blackboardComp->GetValueAsBool( m_dead )
	*   Date altered    : 04/08/2022
	*   Contributors    : Jaber Ahmed
	*   Notes           : N/A
	*   See also        : Behaviour Tree - BT_AssaultSoldier
	*********************************************************************************/
	bool GetDead();

	void DecremenetCover();
	int GetChangeCoverNumber();
	void SetChangeCoverNumber( int integer );

};
