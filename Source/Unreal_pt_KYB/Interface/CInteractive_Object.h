// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CInteractableObject.h"
#include "CInteractive_Object.generated.h"

UCLASS()
class UNREAL_PT_KYB_API ACInteractive_Object : public AActor, public ICInteractableObject
{
	GENERATED_BODY()
	
public:	
	ACInteractive_Object();

protected:
	UPROPERTY(EditAnywhere, Category = "Interactive")
	bool isdisposable = false;

protected:
	virtual void BeginPlay() override;

public:
	// ICInteractableObject�� ���� ��ӵ�
	void Interact() override;

	UFUNCTION(BlueprintNativeEvent, Category = "Interactive")
	void OnInteract();
	virtual void OnInteract_Implementation();

protected:
	//isdisposable�� true�� ��츸 ����ؾ��Ѵ�.
	UPROPERTY(BlueprintReadOnly)
	bool bOnaction = false;

};
