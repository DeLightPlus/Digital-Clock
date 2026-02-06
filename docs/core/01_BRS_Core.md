# Business Requirements Specification (BRS)
Project: iGO Core Smart Dock / Desktop Buddy
Version: 1.0
Date: 2026-02-06
Owner: Kabelo Matlakala

## 1. Purpose
Define the business goals, scope, and success criteria for the iGO Core smart dock/desktop buddy. This document is the business foundation for product, engineering, and go-to-market work.

## 2. Background and Problem
Users rely on multiple devices and apps for time, reminders, and notifications. In markets with unreliable power and expensive data, these systems fail when they are needed most. The core need is a reliable, offline-capable, always-visible assistant that works during power loss and syncs when connectivity returns.

## 3. Business Goals
- Provide a reliable, offline-capable desktop companion for time, reminders, and daily routines.
- Offer a modular platform that can grow into add-on domains (home, farm, city) without replacing hardware.
- Deliver an affordable product aligned to local purchasing power.
- Create a repeatable upgrade path and ecosystem for long-term revenue.

## 4. Scope (Core Only)
In scope for Core:
- Time, alarms, timers, stopwatch
- Notifications and reminders
- Task list and notes (basic)
- Calendar sync (read-only to device, editable via apps)
- Load-shedding / power-outage awareness and battery backup
- Multi-device access (USB desktop app, WiFi web app, BLE mobile app)
- Device settings and customization

Out of scope for Core:
- Smart home automation
- Smart farm monitoring
- Smart city infrastructure
- Medical diagnostics or regulated clinical workflows

## 5. Target Users
- Students and professionals who need reliable scheduling and focus support
- Small business owners who need visible reminders and power-aware alerts
- Households impacted by frequent power cuts

## 6. Value Proposition
- Always-on display for critical information
- Works offline and during power loss
- Low cost, no mandatory subscriptions
- Modular growth path without hardware waste

## 7. Stakeholders
- Founder/Owner
- Product and engineering teams
- Pilot users and beta testers
- Manufacturing and assembly partners

## 8. Assumptions
- Core device uses ESP32-class hardware with RTC and battery backup
- Mobile and desktop clients will be available in early pilot form
- Users accept a simple UI if reliability and power resilience are strong

## 9. Constraints
- Low BOM cost target to meet affordability goals
- Limited compute/storage on embedded device
- Offline-first by design

## 10. Success Metrics
- Daily active usage: >= 60% of pilot users
- Alarm reliability during power outage: 99.5%
- User satisfaction (NPS): >= 50
- Unit cost (BOM): <= target threshold defined in PRD

## 11. Risks
- Hardware supply chain variability
- Over-scoping features beyond embedded limits
- User expectations shaped by smartphone app polish

## 12. Dependencies
- Reliable RTC integration
- Battery and power management module
- Basic mobile and desktop clients for setup and sync

## 13. Next Steps
- Translate BRS into PRD and SRS
- Finalize MVP scope
- Start pilot build and feedback cycle
