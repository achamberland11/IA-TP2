//
// Created by Antoine on 2026-03-07.
//

#pragma once


#define GENERATE_COMPONENT(CanDuplicate, CanBeDisabled, CanBeDeleted) \
static constexpr bool bCanDuplicate = CanDuplicate; \
static constexpr bool bCanBeDisabled = CanBeDisabled; \
static constexpr bool bCanBeDeleted = CanBeDeleted;
