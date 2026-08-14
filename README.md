# 🗳️ Politic Sim — State-Level Legislative State Machine & Electoral Bayesian Filter

[![Live Demo](https://img.shields.io/badge/Live_Showcase-GitHub_Pages-fbbf24?style=for-the-badge&logo=github)](https://jirnyak.github.io/politic_sim/)
[![AI Index](https://img.shields.io/badge/LLM_Search-llms.txt-38bdf8?style=for-the-badge)](https://raw.githubusercontent.com/Jirnyak/politic_sim/main/llms.txt)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-00599C?style=for-the-badge&logo=cplusplus)](https://isocpp.org/)
[![Game Theory](https://img.shields.io/badge/Theory-Shapley_Shubik_Power-f43f5e?style=for-the-badge)](https://en.wikipedia.org/wiki/Shapley%E2%80%93Shubik_power_index)

A deterministic political ecosystem simulator modeling legislative bill passage, coalition formation via Shapley-Shubik voting power indices, and Bayesian polling update filters across multi-party parliaments.

---

## 🏛️ Legislative State Machine

```mermaid
graph LR
    Bill[Proposed Legislation] --> Committee[Legislative Committee Review]
    Committee --> Amend[Amendments & Pork-Barrel Bargaining]
    Amend --> Coalition[Shapley-Shubik Coalition Building]
    Coalition --> Vote{Parliamentary Floor Vote}
    Vote -->|Pass| Law[Statutory Law Inacted]
    Vote -->|Fail| Discard[Bill Rejected]
```

---

## 🔬 Core Capabilities

1. **Shapley-Shubik Voting Power:** Mathematical computation of pivotal party leverage in fragmented multi-party parliaments.
2. **Bayesian Polling Updates:** Real-time belief updating of voter preference distributions following economic shocks.
3. **Ideological Compass Space:** 2D spatial voting models (Economic Left/Right vs Authoritarian/Libertarian).

---

### 👨‍💻 Engineering Syndicate & Authors
- **Жирняк (Jirnyak)** — Lead Political Scientist & Game Theory Engine.  
  GitHub: [@Jirnyak](https://github.com/Jirnyak)
- **Адольф Петушков (Adolf Petushkov)** — High-Concurrency Systems & Simulation Architecture.  
  GitHub: [@marko1olo](https://github.com/marko1olo)
