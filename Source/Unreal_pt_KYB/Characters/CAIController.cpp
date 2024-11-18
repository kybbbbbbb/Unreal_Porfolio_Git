#include "Characters/CAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "CMonsters_AI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "../Components/CAIBehaviorComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "CPlayer.h"

ACAIController::ACAIController()
{
	Perception = CreateDefaultSubobject<UAIPerceptionComponent>("Perception");

	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackBoard");
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	DamageConfigs = CreateDefaultSubobject<UAISenseConfig_Damage>("DamageSense");
	DamageConfigs->SetMaxAge(30.0f);

	Perception->ConfigureSense(*Sight);
	Perception->ConfigureSense(*DamageConfigs);
}



void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	SetSightConfigByIndex(0);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdate);
}


void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Monster = Cast<ACMonsters_AI>(InPawn);
	if (Monster != nullptr)
	{
		SetGenericTeamId(Monster->GetTeamID());
	}

	if (Monster->GetBeheviorTree() == nullptr)
		return;

	UBlackboardComponent* blackboard = Blackboard.Get();

	UseBlackboard(Monster->GetBeheviorTree()->BlackboardAsset, blackboard);
	RunBehaviorTree(Monster->GetBeheviorTree());

	AIBehavior = Monster->GetAIBehevior();
	AIBehavior->SetBlackboard(blackboard);
	

}

void ACAIController::OnUnPossess()
{

	Super::OnUnPossess();
}



void ACAIController::OnPerceptionUpdate(const TArray<AActor*>& UpdatedActors)
{
	if (AIBehavior->IsDeadMode())
	{
		Dead();
		return;
	}



	TArray<AActor*> actors;
	//������ ������
	Perception->GetCurrentlyPerceivedActors(UAISense_Damage::StaticClass(), actors);
	if (actors.Num() > 0)
	{
		if (AIBehavior->IsDeadMode())
		{
			Dead();
			return;
		}
		for (int32 i = 0; i < actors.Num(); i++)
		{
			if (actors[i]->IsA(ACPlayer::StaticClass()))
			{
				ACPlayer* player = Cast<ACPlayer>(actors[i]);
				if (CurrentPlayerSave == nullptr)
				{
					bPlayerAttackable = player->RequestAttackToken();
					CurrentPlayerSave = player;
					Blackboard->SetValueAsObject("Target", player);
					return;
				}

				if (CurrentPlayerSave != nullptr)
				{
					if (bPlayerAttackable == false)
					{
						//�÷����� ���� �ٽ� üũ
						bPlayerAttackable = player->RequestAttackToken();
						return;
					}

				}
			}
		}
	}





	//�þ߿��� ���� ����� ���� ����
	Perception->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), actors);

	AActor* NearestTarget = nullptr;
	float MinDistance = TNumericLimits<float>::Max();  

	for (AActor* Actor : actors)
	{

		float Distance = FVector::Dist(Actor->GetActorLocation(), GetPawn()->GetActorLocation());

		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			NearestTarget = Actor;
		}
		
	}

	//������ Ÿ���� ������
	if (NearestTarget == nullptr)
	{
		//����Ÿ���� ���� �÷��̾ ����Ȱ� ���ٸ�
		if (CurrentPlayerSave == nullptr)
		{
			if (bPlayerAttackable == true)
			{
				bPlayerAttackable = false;
			}

			Blackboard->SetValueAsObject("Target", nullptr);
			return;
		}

		//����Ÿ���� ������ �÷��̾ ����Ȱ� �ִٸ�
		if (CurrentPlayerSave != nullptr)
		{
			if (bPlayerAttackable == true)
			{
				ACPlayer* player = Cast<ACPlayer>(CurrentPlayerSave);
				player->ReleaseAttackToken();
				bPlayerAttackable = false;
				CurrentPlayerSave = nullptr;
				Blackboard->SetValueAsObject("Target", nullptr);
				return;
			}

			if (bPlayerAttackable == false)
			{
				Blackboard->SetValueAsObject("Target", nullptr);
				CurrentPlayerSave = nullptr;
				return;
			}
		}

	}


	// ���� ����� Ÿ���� ����
	if (NearestTarget)
	{



		//���� ����� �÷��̾ ������ ����
		if (CurrentPlayerSave == nullptr)
		{
			//����� �÷��̾ ���� ���� Ÿ���� �÷��̾��?
			if (NearestTarget->IsA(ACPlayer::StaticClass()))
			{
				ACPlayer* player = Cast<ACPlayer>(NearestTarget);
				bPlayerAttackable = player->RequestAttackToken();
				CurrentPlayerSave = player;

				Blackboard->SetValueAsObject("Target", player);
				return;

			}
			else //Ÿ���� �÷��̾ �ƴϸ�
			{
				Blackboard->SetValueAsObject("Target", NearestTarget);
				return;
			}
		}




		//���� ����� �÷��̾ �����Ѵٸ�
		if (CurrentPlayerSave != nullptr)
		{
			
			//����� �÷��̾ �����ϰ� ���� Ÿ���� ������ �÷��̾��?
			if (NearestTarget == CurrentPlayerSave)
			{
				
				//���� �÷��̾� ���� �Ұ��� ���¶�� (�����)
				if (bPlayerAttackable == false)
				{
					//�÷����� ���� �ٽ� üũ
					ACPlayer* player = Cast<ACPlayer>(CurrentPlayerSave);
					bPlayerAttackable = player->RequestAttackToken();
					return;
				}

				//���� �÷��̾� ���� ���ɻ����̸�
				if(bPlayerAttackable == true)
					return;
			}
			else //����� �÷��̾ �����ϰ� ���� Ÿ���� �÷��̾ �ƴϸ�?
			{
				if (bPlayerAttackable == false)
				{
					Blackboard->SetValueAsObject("Target", NearestTarget);
					CurrentPlayerSave = nullptr;
					return;
				}

				if (bPlayerAttackable == true)
				{
					ACPlayer* player = Cast<ACPlayer>(CurrentPlayerSave);
					Blackboard->SetValueAsObject("Target", NearestTarget);
					player->ReleaseAttackToken();
					CurrentPlayerSave = nullptr;
					bPlayerAttackable = false;
					return;
				}
			}
		}
	}



}



void ACAIController::SetSightConfigByIndex(int32 Index)
{
	if (SightConfigs.IsValidIndex(Index))
	{
		const FSightConfig& Config = SightConfigs[Index];

		Sight->SightRadius = Config.SightRadius;
		Sight->LoseSightRadius = Config.LoseSightRadius;
		Sight->PeripheralVisionAngleDegrees = Config.PeripheralVisionAngleDegrees;

		Sight->DetectionByAffiliation.bDetectEnemies = Config.bDetectEnemies;
		Sight->DetectionByAffiliation.bDetectNeutrals = Config.bDetectNeutrals;
		Sight->DetectionByAffiliation.bDetectFriendlies = Config.bDetectFriendlies;

		Perception->ConfigureSense(*Sight);
	}
}

void ACAIController::Dead()
{

	if (CurrentPlayerSave == nullptr)
	{
		Blackboard->SetValueAsObject("Target", nullptr);
		return;
	}

	if (CurrentPlayerSave != nullptr)
	{
		if (bPlayerAttackable == true)
		{
			ACPlayer* player = Cast<ACPlayer>(CurrentPlayerSave);
			player->ReleaseAttackToken();
			bPlayerAttackable = false;
			CurrentPlayerSave = nullptr;
			Blackboard->SetValueAsObject("Target", nullptr);
			return;
		}

		if (bPlayerAttackable == false)
		{
			Blackboard->SetValueAsObject("Target", nullptr);
			CurrentPlayerSave = nullptr;
			return;
		}
	}
	return;

}
