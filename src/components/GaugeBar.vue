<template>
  <div class="gauge-bar" :style="{'height': totalHeight}">
      <div v-if="showTotal" class="max-value" :style="{'font-size': textSize, 'top': -textSize - gap}">{{ maxDisplay }}</div>
      <div class="gauge-border" :style="{'border-width': borderWidth, 'border-color': border, 'padding': gap}">
          <div class="gauge-fill" :style="{'width': pctDisplay, 'background': fill}">
              <div class="current-value" :style="innerTextStyle">{{ currentDisplay }}</div>
          </div>
          <div class="pct-value" :style="innerTextStyle">{{ pctDisplay }}</div>
      </div>
  </div>
</template>

<script>
export default {
  name: 'gauge-bar',
  props: {
    borderColour: String,
    fillColour: String,
    dangerColour: String,
    dangerThreshold: Number,
    maxValue: Number,
    minValue: Number,
    currentValue: Number,
    valueFormatter: Function,
    showPercent: Boolean,
    barHeight: Number,
    borderThickness: Number,
    borderGap: Number,
    showTotal: Boolean,
  },

  created() {
    this.formatText = val => {
      return this.valueFormatter ? this.valueFormatter(val) : val.toString();
    };
  },

  computed: {
    min() {
      return this.minValue || 0;
    },
    max() {
      return this.maxValue || 100;
    },
    current() {
      return this.currentValue || 0;
    },
    minDisplay() {
      return this.valueFormatter
        ? this.valueFormatter(this.min)
        : this.min.toString();
    },
    maxDisplay() {
      return this.valueFormatter
        ? this.valueFormatter(this.max || 0)
        : this.max.toString();
    },
    currentDisplay() {
      return this.formatText(this.current);
    },
    pct() {
      return this.max && this.current ? this.current / this.max : 0;
    },
    pctDisplay() {
      return `${(this.pct * 100).toFixed()}%`;
    },
    border() {
      return this.borderColour || '#2c3e50';
    },
    fill() {
      return this.pct > (this.dangerThreshold || 0.9)
        ? this.dangerColour || '#b30000'
        : this.fillColour || '#2c3e50';
    },
    gap() {
      return this.borderGap || 2;
    },
    borderWidth() {
      return this.borderThickness || 4;
    },
    totalHeight() {
      const bar = this.barHeight || 40;
      return bar + 2 * this.borderWidth + 2 * this.gap;
    },
    textSize() {
      return this.totalHeight * 0.5;
    },
    innerTextStyle() {
      return {
        'font-size': this.textSize,
        'padding-top': (this.totalHeight - this.textSize) / 4,
      };
    },
  },
};
</script>


<style lang="less">
.gauge-bar {
  position: relative;
  width: 100%;
  .gauge-border {
    position: absolute;
    top: 0;
    left: 0;
    bottom: 0;
    right: 0;
    border-style: solid;

    .gauge-fill {
      height: 100%;
      .current-value {
        font-weight: bold;
        color: #fff;
        text-align: center;
      }
    }

    .pct-value {
      text-align: right;
      font-weight: bold;
      position: absolute;
      right: 5;
      top: 0;
    }
  }

  .max-value {
    font-weight: bold;
    position: absolute;
    right: 0;
  }
}
</style>

