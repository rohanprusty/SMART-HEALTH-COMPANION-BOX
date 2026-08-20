# SAARTHI (सारथि) — Research Brief & Pitch Deck
### Smart Health Companion Box: An Automated Medication Reminder & Dispensing System

*Prepared for: technical evaluation, patent filing support, and placement/interview presentation*
*Team: Rohan (24BEC104), Aftab (24BEC076), Dhruv (24BEC032), Prachetus (24BEC093), Samarth (24BEC107) — IIIT Jabalpur*

---

## 1. Executive Summary

SAARTHI is a low-cost, IoT-connected medication box that physically locks
patients' medicine compartments and only releases them at the scheduled dose
time — using an audible-visual alarm plus a Blynk-app-linked caregiver
dashboard for remote monitoring. It sits at the intersection of two large,
well-funded but poorly-served problems: **medication non-adherence** (a
multi-billion-dollar healthcare cost) and **affordable elder/patient care
technology** (where the leading products cost 20–100x what SAARTHI is built
for). The pitch, in one line:

> **"We built the $1,999 medication-adherence problem a $20 hardware
> solution — with the caregiver app the $50/month competitors charge for."**

---

## 2. The Problem: Why This Matters (Evidence Base)

Medication non-adherence is one of the most studied, most expensive, and
most under-addressed problems in global healthcare:

- Adherence to long-term medication therapy in high-income countries
  averages only around **50%**, and is lower still in lower-income
  countries, according to WHO-cited research.
- Global estimates put roughly **200,000 preventable deaths and hospitalizations**
  a year, and well over **$100–300 billion in avoidable annual healthcare
  costs**, at the door of missed or incorrect dosing.
- Non-adherence disproportionately affects the exact population SAARTHI
  targets: elderly patients managing multiple daily medications, where
  memory lapses and complex schedules compound the risk.
- The global **medication adherence market** itself is valued at roughly
  **$3.9–4.3 billion (2024–25)**, growing at a **~10% CAGR**, confirming this
  isn't a hypothetical problem — hospitals, insurers, and families are
  already paying to solve it.

The gap SAARTHI targets: existing smart solutions (below) solve this for
patients who can pay $500–$2,500 upfront or $30–$125/month. That price band
excludes the overwhelming majority of patients in India and other
price-sensitive markets — a gap a low-BOM-cost ESP8266 build is well
positioned to close.

*(Sources: WHO-cited adherence research via the 2024 a:care congress
summary; medication non-adherence cost/mortality figures reported in
US patent filings and pharmacy-industry analyses; medication adherence
market sizing from ResearchAndMarkets/The Business Research Company,
2025. Full links in [References](#9-references).)*

---

## 3. The Solution & Its Novelty

### 3.1 What SAARTHI does differently

| Capability | Typical low-cost pill organizers | Typical premium smart dispensers (Hero, MedMinder, Medacube) | **SAARTHI** |
|---|---|---|---|
| Reminder alarm | ✅ (basic) | ✅ | ✅ |
| Physical lock until dose time | ❌ | ✅ | ✅ |
| Remote caregiver app | ❌ | ✅ (subscription) | ✅ (Blynk, no subscription) |
| Live pill-count sync | ❌ | ✅ | ✅ |
| Auto interior lighting | ❌ | Rare | ✅ |
| Hardware cost | Very low | Very high | **Low** |
| Ongoing fees | None | $30–$125/month | **None (planned)** |

### 3.2 Core novelty claims (useful for the patent write-up)

1. **Single-button dual-function interaction** — one push button both
   *acknowledges* the dose (silencing the alarm, logging compliance, and
   decrementing pill count) *and* actuates the physical lock, collapsing what
   competitors do with multiple sensors/touchscreens into one low-cost
   mechanism.
2. **Segmented compartment design synced to a digital ledger** — the 6
   physical compartments are mapped 1:1 to counts tracked in the cloud, so
   the physical and digital pill inventory never drift apart without needing
   weight sensors or cameras (which is where most of the cost in premium
   dispensers goes).
3. **Fully event-driven state machine on commodity hardware** — the
   alarm/lock/lighting/logging sequence runs entirely on a ~$3 microcontroller
   (NodeMCU ESP8266) with no proprietary hub, no cellular modem, and no
   recurring cloud bill — which is what lets the price point undercut every
   competitor by an order of magnitude.
4. **Caregiver-configurable, patient-passive design** — the patient never
   needs to interact with an app; all configuration happens caregiver-side,
   which matters for the elderly/cognitively-impaired users who are the
   primary target and who are the group least able to manage a smartphone
   interface themselves.

*(Note: "novelty" here is drafted as talking points for pitches and as a
starting point for your patent attorney — it is not a substitute for a
formal patentability/prior-art search, which your attorney or a patent
agent should run before or alongside filing.)*

---

## 4. Target Market

### 4.1 Segments (largest to most specialized)

1. **Elderly patients on multi-drug regimens** (primary) — chronic-disease
   patients (diabetes, hypertension, cardiac) managing 3+ daily medicines,
   often living independently or with a distant caregiver.
2. **Distant caregivers / adult children** — the actual app users and buyers
   in most cases; they want proof of adherence without a phone call.
3. **Assisted-living and old-age homes** (institutional) — a facility could
   deploy one unit per resident, with a single staff dashboard aggregating
   many boxes (a natural B2B upsell once the core hardware is proven).
4. **Post-surgical / short-course compliance** (e.g., antibiotics, TB/DOTS
   programs) — a rentable/disposable-tier use case in public health programs,
   where strict short-term adherence has outsized outcome impact.
5. **Hospitals & pharmacies** as a channel partner — bundled with discharge
   kits for high-risk patients (heart failure, post-op) as a readmission-
   reduction tool.

### 4.2 Why India-first makes sense

- India's out-of-pocket healthcare spending is high and $30–125/month
  subscription devices are a non-starter for the mass market; a one-time,
  low-cost hardware purchase model fits local willingness to pay far better.
- A large and growing elderly population with adult children working
  in different cities is exactly the "distant caregiver" pattern the product
  is built around.
- Manufacturing at low BOM cost is a genuine India-based advantage — local
  sourcing of NodeMCU, servos, and LCD modules keeps landed cost low without
  needing to compete with the US-based premium players on their own turf.

---

## 5. Price vs. Product: Competitive Analysis

| Product | Model | Price | What you get |
|---|---|---|---|
| **Hero** | Subscription | ~$30–$60/month (or $1,999+ outright) | App, dispensing, caregiver alerts, no cellular reqd. beyond WiFi |
| **MedMinder** | Subscription | ~$50–$125/month + ~$100 setup | Cellular, pillbox form factor, caregiver calls |
| **Medacube** | One-time | ~$1,299–$1,999 | No subscription, but high upfront cost |
| **Livi** | One-time | ~$979 | Outright purchase, limited app features |
| **LiveFine / basic dispensers** | One-time | ~$60–$90 | Reminders only, no lock, no remote app |
| **SAARTHI (this project)** | One-time, no subscription (planned) | **Estimated BOM ≈ ₹1,000–1,800 (~$12–22)**; target retail in the ₹2,500–4,000 (~$30–48) range | Locking mechanism, caregiver app (Blynk), live pill-count sync, interior lighting — **no recurring fee** |

**Estimated Bill-of-Materials cost** *(placeholder — replace with your
actual component invoices before presenting; approximate India retail
prices at prototype-scale, single-unit quantities)*:

| Component | Approx. cost (₹) |
|---|---|
| NodeMCU ESP8266 | 250–300 |
| 16x2 I2C LCD | 150–200 |
| SG90 Servo | 100–150 |
| Push button, buzzer, LED strip | 80–120 |
| 18650 battery + charging module | 250–350 |
| Enclosure (3D printed / laser cut) | 200–400 |
| **Total (est.)** | **~₹1,000–1,800** |

**The pitch line:** *"Every competitor charges a monthly fee that adds up to
thousands of dollars a year. We built the same core problem-solving loop —
alarm, lock, caregiver visibility — onto sub-$25 hardware with no recurring
cost, which is the only version of this product that actually works at
Indian price points and at institutional scale."*

*(Sources for competitor pricing: Hero, MedMinder, Medacube, and Livi
pricing as publicly listed by Hero Health, TheSeniorList, and
SeniorSite comparison guides, 2025–2026. Full links in [References](#9-references).
Always re-verify current prices before quoting them live — subscription
pricing changes frequently.)*

---

## 6. Business Model Options

1. **One-time hardware sale** (simplest, matches India price sensitivity) —
   margin from manufacturing efficiency at scale, no recurring infrastructure
   cost since Blynk's free tier supports low-device-count deployments.
2. **Hardware + optional premium app tier** — base hardware sold outright;
   an optional paid tier unlocks multi-caregiver accounts, SMS/call alerts,
   or analytics for institutional buyers (old-age homes, hospitals).
3. **B2B2C via hospitals/NGOs** — bundled into discharge kits or public
   health compliance programs (e.g., TB/DOTS), priced per unit to the
   institution rather than the individual patient.
4. **Institutional dashboard licensing** — a facility-wide dashboard
   aggregating many boxes, licensed per-bed to assisted-living facilities.

---

## 7. Future Enhancements & Roadmap

| Phase | Enhancement | Why it matters |
|---|---|---|
| Near-term | **Biometric security** (fingerprint sensor) | Ensures only the authorized patient opens the box — critical for controlled substances and shared households. |
| Near-term | **Deep-sleep power management** | Extends battery life from days to months, a major usability blocker today. |
| Mid-term | **ESP32-CAM ingestion verification** | Moves from "box was opened" to "medication was actually taken" — a meaningfully stronger adherence signal for clinical/insurance use cases. |
| Mid-term | **Voice assistant integration** (Alexa/Google via Blynk webhooks) | Removes reliance on reading a small LCD — useful for visually impaired elderly users. |
| Long-term | **Multi-unit institutional dashboard** | Unlocks the B2B assisted-living/hospital channel described above. |
| Long-term | **Regulatory pathway exploration** (e.g., as a Class-A/low-risk medical device where applicable) | Needed for hospital procurement and insurance reimbursement conversations. |

---

## 8. Risks & Honest Caveats

*(Worth having answers ready for — judges and interviewers will ask these.)*

- **Hardware reliability at scale** — a servo-locked mechanism needs to be
  stress-tested for thousands of open/close cycles before claiming
  production-readiness.
- **WiFi dependency** — the current design needs home WiFi; rural or
  low-connectivity deployments would need a cellular or offline-alarm fallback.
- **Security of the auth token/credentials** — the current firmware hardcodes
  WiFi and Blynk credentials in source; this needs to move to a
  provisioning flow before any real-world or public deployment (also flagged
  in the README).
- **Not yet a certified medical device** — dosing/adherence claims should be
  framed as a reminder and logging aid, not a diagnostic or prescribing tool,
  until any relevant regulatory review is complete.
- **Patent status is "applied," not "granted"** — be precise about this
  distinction when presenting; "patent applied for" is accurate, "patented"
  is not, until it actually issues.

---

## 9. References

- WHO-cited adherence statistics — *"The silent epidemic of non-adherence,"* NCBI/PMC, 2024 a:care congress summary: https://www.ncbi.nlm.nih.gov/pmc/articles/PMC12096470/
- Non-adherence cost/mortality figures cited in a US patent filing — USPTO: https://image-ppubs.uspto.gov/dirsearch-public/print/downloadPdf/10083594
- Medication adherence market sizing (2025) — ResearchAndMarkets via GlobeNewswire: https://finance.yahoo.com/news/medication-adherence-research-report-2025-152600258.html
- Medication adherence market sizing (2024–25 growth) — The Business Research Company: https://www.giiresearch.com/report/tbrc1824503-medication-adherence-global-market-report.html
- Hero pricing and outright-purchase competitor pricing (Livi, Medacube, Philips, MedMinder): https://herohealth.com/pill-reminder/ and https://herohealth.com/pricing/
- MedMinder / dispenser cost comparison: https://www.theseniorlist.com/medication/dispensers/
- Automatic pill dispenser cost/feature comparison (2026): https://caringvillage.com/product-reviews/automatic-pill-dispensers/
- Automatic pill dispenser guide and MedaCube pricing: https://seniorsite.org/resource/9-easy-to-use-automatic-pill-dispensers-for-elderly-parents

---

## 10. 60-Second Elevator Pitch (Script)

> *"Nearly half of all patients on long-term medication don't take it as
> prescribed — and it costs the world over a hundred billion dollars a year
> in hospital visits that never needed to happen. The products that actually
> fix this — Hero, MedMinder — cost thirty to a hundred and twenty-five
> dollars a month. That's not a solution for most of the world; that's a
> luxury good.*
>
> *We built SAARTHI — a locking, alarming, app-connected medicine box — on
> under twenty-five dollars of hardware, with zero subscription fee. It
> locks each dose until it's time, alerts the patient, and tells a
> caregiver anywhere in the world the moment a dose is taken or missed. One
> button. No app for the patient to learn. Everything a two-thousand-dollar
> device does, at a price an actual family — or an actual hospital, at
> scale — can afford. We're a team of five ECE students at IIIT Jabalpur, and
> we've already filed for a patent on the mechanism. We're not asking you to
> imagine the market — it's a four-billion-dollar market today, growing at
> ten percent a year. We're asking you to imagine it finally being
> affordable."*

---

## 11. Quick-Reference FAQ (for interviews/placements)

**Q: What's the actual innovation, not just "IoT pill box"?**
A: The combination of a single-button dose-acknowledgment-and-unlock
mechanism with a live-synced digital pill ledger, built entirely on
commodity components — that's what gets the cost down by an order of
magnitude versus existing "smart dispenser" products, without dropping the
features that matter (lock, remote visibility, live counts).

**Q: Who pays for this?**
A: Primary: the adult child/caregiver of an elderly parent, as a one-time
purchase. Secondary: assisted-living facilities and hospitals, as a
per-unit institutional purchase, once a multi-unit dashboard exists.

**Q: What's stopping a big player from just copying this?**
A: Nothing structurally — the technical moat here is cost discipline and
the patent-pending mechanism, not exotic hardware. The real defensibility
would come from being first to an underserved, price-sensitive market
segment the premium players have no incentive to chase.

**Q: Is this a medical device?**
A: Today it's a reminder-and-logging aid, not a diagnostic or prescribing
device — that framing matters for both legal accuracy and for what claims
can honestly be made in a pitch.
