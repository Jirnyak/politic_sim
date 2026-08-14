# 🛠️ Contributing to Jirnyak/politic_sim

> **Engineering Guidelines, Architecture Invariants & Pull Request Lifecycle**  
> Maintained by the **Жирняк & Адольф Петушков** Engineering Syndicate

Thank you for your interest in contributing to **Jirnyak/politic_sim**. This project operates under strict technical standards: deep mathematical and domain correctness, zero-slop code, explicit typing, and zero unverified assumptions.

---

## 🏛️ 1. Core Engineering Invariants

Before proposing any changes, verify that your implementation satisfies our domain invariants:

1. **Zero-Sum Resource Balance**:  Total money and raw material in the global economy must satisfy conservation laws.
2. **Fractional Voting Matrix**:  Parliamentary seat distribution must use deterministic Sainte-Laguë or D'Hondt algorithms.
3. **Faction Sentiment Decay**:  Public opinion shifts must follow smooth non-linear sigmoid response curves.
4. **Deterministic Event Queue**:  Historical and random geopolitical events must execute in strict chronological sequence.

---

## 💻 2. Local Development & Toolchain

### 2.1 Prerequisites
* **Tech Stack**: `TypeScript / JavaScript / Canvas 2D / Web Audio`
* Ensure your compiler / runtime matches the repository configuration exactly.

### 2.2 Setup Workflow
```bash
# Clone the repository
git clone https://github.com/Jirnyak/politic_sim.git
cd politic_sim

# Install dependencies / configure build
npm install # or make / dotnet restore depending on project

# Run the test suite
npm test || node test.js
```

---

## 📐 3. Coding Standards & Style

1. **Zero AI-Slop & Filler**:
   * Do NOT add generic, conversational comments (e.g. `// This function handles...`, `// This is useful because...`).
   * Code must be self-explanatory through precise naming, mathematical clarity, and strong types.
   * Only document non-obvious mathematical invariants, hardware quirks, or algorithmic complexity bounds.

2. **Strong Typing & Strict Validation**:
   * Zero `any`, `unknown` bypasses, or untyped data flows.
   * All external inputs, network payloads, and deserialized states must pass strict schema validation at the system boundary.

3. **Performance & Memory Hygiene**:
   * Render and simulation loops must produce zero heap allocations per frame.
   * Reuse pre-allocated buffers, typed arrays, or object pools.
   * Guarantee deterministic cleanup of native resources, file handles, and event listeners.

---

## 🧪 4. Testing & Verification Requirements

Every pull request must be accompanied by empirical proof of correctness:
1. **Unit Tests**: Add targeted tests covering both the nominal path and boundary edge cases.
2. **Regression Verification**: Ensure all existing test suites pass cleanly with `npm test || node test.js`.
3. **No Mocks in Core Solvers**: Domain logic must be tested against real mathematical and architectural invariants, not mock interfaces.

---

## 🚀 5. Pull Request & Review Protocol

```mermaid
graph LR
    A[Fork & Create Branch] --> B[Implement Fix / Feature]
    B --> C[Pass Local Test Suite]
    C --> D[Submit PR with Detailed Rationale]
    D --> E[Syndicate Review & CI Matrix]
    E -->|Approved| F[Squash & Merge to main]
    E -->|Changes Requested| B
```

1. **Branch Naming**: Use descriptive prefixes: `fix/<issue-name>`, `feat/<feature-name>`, `perf/<optimization>`.
2. **Commit Messages**: Follow Conventional Commits format: `fix(subsystem): brief summary of change`.
3. **PR Description**: Include:
   * Root cause analysis of the bug or architectural rationale for the feature.
   * Exact commands used to verify correctness and raw test output snippets.
   * Confirmation that no unrelated files or stylistic diffs were introduced.

---

### 👥 Engineering Syndicate
Maintained by **Жирняк** & **Адольф Петушков**.
